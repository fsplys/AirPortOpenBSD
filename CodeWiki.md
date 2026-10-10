# AirPortOpenBSD Code Wiki

> 本文档为 AirPortOpenBSD 项目的结构化代码 Wiki，涵盖项目整体架构、主要模块职责、关键类与函数说明、依赖关系及项目运行方式等关键信息。

---

## 目录

1. [项目概述](#1-项目概述)
2. [整体架构](#2-整体架构)
3. [目录结构](#3-目录结构)
4. [主要模块职责](#4-主要模块职责)
5. [关键类与函数说明](#5-关键类与函数说明)
6. [核心数据结构](#6-核心数据结构)
7. [依赖关系](#7-依赖关系)
8. [项目运行与构建方式](#8-项目运行与构建方式)
9. [核心工作流程](#9-核心工作流程)
10. [版本演进与支持硬件](#10-版本演进与支持硬件)

---

## 1. 项目概述

**AirPortOpenBSD** 是一个 macOS 内核扩展（Kernel Extension，kext），其核心目标是将 OpenBSD 项目中的无线网卡驱动及其配套的 net80211 协议栈移植到 macOS 内核环境中运行，从而使非原生支持的 Wi-Fi 网卡（主要是 Intel 系列及部分 Broadcom/Realtek 芯片）能够在 macOS 上工作。

### 核心特性

- 尽可能保留 OpenBSD 原始代码的完整性，便于同步上游更新
- 支持原生 macOS Wi-Fi 客户端（依赖 IO80211Family）
- 同时提供 IOEthernet 变体（不依赖 IO80211Family，兼容旧系统）
- 支持的认证方式：OPEN、WEP、WPA-PSK、WPA2-PSK、WPA2-Enterprise（802.1X）
- 固件以 C 数组形式内嵌编译，无需运行时加载外部固件文件
- 已迁移驱动：`iwm`、`iwx`、`iwn`、`ipw`、`iwi`、`wpi`、`bwfm`、`bwi`、`rtwn`（其中 `iwm`、`iwx` 已通过测试）

### 参考来源

- [OpenBSD src](https://github.com/openbsd/src)（驱动与协议栈来源）
- [mercurysquad/Voodoo80211](https://github.com/mercurysquad/Voodoo80211)
- [AppleIntelWiFi/Black80211-Catalina](https://github.com/AppleIntelWiFi/Black80211-Catalina)
- [rpeshkov/black80211](https://github.com/rpeshkov/black80211)
- [Mieze/IntelMausiEthernet](https://github.com/Mieze/IntelMausiEthernet)
- [OpenIntelWireless/itlwm](https://github.com/OpenIntelWireless/itlwm)

---

## 2. 整体架构

项目采用**分层适配架构**，自上而下分为三层：

```
┌─────────────────────────────────────────────────────────┐
│              macOS 内核 / IOKit 框架                     │
│   (IO80211Family / IONetworkingFamily / IOPCIFamily)    │
├─────────────────────────────────────────────────────────┤
│         macOS 适配层 (AirPortOpenBSD*.cpp/hpp)           │
│  - AirPortOpenBSD 类: IO80211Controller 子类              │
│  - apple80211 IOCTL 请求分发                             │
│  - 固件加载 / 数据包收发 / 电源管理                       │
├─────────────────────────────────────────────────────────┤
│         OpenBSD 兼容层 (openbsd/sys, machine, netinet)   │
│  - ifnet 结构体 (macOS-OpenBSD 桥梁)                     │
│  - bus_space / bus_dma / mutex / task / timeout 适配     │
│  - mbuf / malloc / PCI 配置访问适配                      │
├─────────────────────────────────────────────────────────┤
│         OpenBSD 协议栈与驱动 (openbsd/net80211, dev/pci) │
│  - net80211: 802.11 协议状态机 / 节点管理 / 加密         │
│  - 驱动: iwm iwx iwn ipw iwi wpi bwfm bwi rtwn          │
├─────────────────────────────────────────────────────────┤
│         OpenBSD 加密库 (openbsd/crypto)                  │
│  - aes sha1 sha2 md5 hmac cmac arc4 michael key_wrap    │
├─────────────────────────────────────────────────────────┤
│         内嵌固件 (firmware/*.c, sources/firmware/*.bin)  │
└─────────────────────────────────────────────────────────┘
```

### 设计要点

- **桥梁结构 `ifnet`**：这是整个移植的核心。OpenBSD 的协议栈和驱动都围绕 `struct ifnet` 运作，项目在其定义中扩展了 macOS IOKit 相关字段（`IOInterface *iface`、`IOWorkLoop *fWorkloop`、`IOCommandGate *fCommandGate`、`IONetworkStats *netStat` 等），使 OpenBSD 代码能在不感知 macOS 的前提下与 IOKit 交互。
- **全局指针 `_ifp`**：指向当前活动的 `ifnet`，作为 OpenBSD 代码访问 macOS 适配层的统一入口。
- **编译宏双形态**：通过 `#ifdef Ethernet` 宏切换，驱动可编译为 `IO80211Controller` 子类（原生 Wi-Fi）或 `IOEthernetController` 子类（以太网接口形态）。
- **固件内嵌**：使用 `fw2bin.sh` 脚本将二进制固件转换为 C 数组编译进 kext，避免运行时依赖外部固件文件。

---

## 3. 目录结构

```
AirPortOpenBSD/
├── AirPortOpenBSD.hpp              # 主类声明（核心头文件）
├── AirPortOpenBSD.cpp              # 主类实现：init/probe/start/stop/生命周期
├── AirPortOpenBSD_80211.cpp        # 802.11 原始帧/BPF 输出实现
├── AirPortOpenBSD_Ethernet.cpp     # Ethernet 变体自动关联逻辑
├── AirPortOpenBSD_Ioctl.cpp        # apple80211 IOCTL 请求分发与各 get/set 处理
├── AirPortOpenBSD_Util.cpp         # 固件加载/数据输入/扫描结果转换等工具
├── compat.cpp / compat.h           # OpenBSD→macOS 兼容函数（tsleep/wakeup/if_input 等）
├── help_ifconfig.cpp / .h          # 内嵌 ifconfig 命令实现（nwid/wpakey/join 等）
├── apple80211.h                    # 双形态宏定义（IO80211 vs IOEthernet）
├── iwlwifi.h                       # 驱动注册表（cfdriver/cfattach 列表）
├── bpfilter.h / bridge.h / vlan.h # 网络子系统兼容桩
├── Info.plist                      # kext 元数据（IO80211 形态）
├── InfoIOEthernet.plist            # kext 元数据（IOEthernet 形态，含 IFConfig 配置）
├── Apple Headers/                  # IONetworkController.h / IONetworkInterface.h
├── apple80211/                     # Apple 802.11 框架私有头文件
│   ├── IO80211Controller.h / IO80211Interface.h
│   ├── IO80211WorkLoop.h / IOSkywalkEthernetInterface.h
│   ├── apple80211_ioctl.h / apple80211_var.h / apple80211_wps.h
│   └── ioctl/                       # Apple ioctl 命令符号定义
├── firmware/                       # 内嵌固件（C 数组）+ 加载逻辑
│   ├── firmware.h / firmware.c / firmwarevar.h
│   ├── brcmfmac*.c / iwm_*.c / iwn_*.c / iwx_*.c / ...
│   └── bwi_airforce.c / ipw_*.c / iwi_*.c / rtwn_*.c / wpi_*.c
├── sources/firmware/               # 原始二进制固件文件（fw2bin 输入）
└── openbsd/                        # OpenBSD 移植代码
    ├── crypto/                     # 加密算法实现
    ├── dev/pci/                    # PCI 设备驱动 + PCI 框架
    ├── dev/ic/                     # IC 级设备驱动（bwfm/bwi/rtwn）
    ├── machine/                    # 总线空间/中断抽象
    ├── net80211/                   # IEEE 802.11 协议栈
    ├── netinet/                    # 网络接口兼容层（ifnet/bpf/if_media）
    ├── sys/                        # 内核数据结构与同步原语兼容
    └── uvm/                        # 虚拟内存兼容（仅头文件桩）
```

---

## 4. 主要模块职责

### 4.1 macOS 适配层（根目录源文件）

| 文件 | 职责 |
|------|------|
| `AirPortOpenBSD.hpp` | 主类 `AirPortOpenBSD` 声明，定义生命周期方法、apple80211 IOCTL 处理函数集、数据收发接口、扫描/固件/电源管理成员变量 |
| `AirPortOpenBSD.cpp` | kext 生命周期实现：`init`/`probe`（PCI 设备匹配）/`start`（初始化 WorkLoop、CommandGate、看门狗定时器、分配 `if_softc`、调用驱动 `ca_attach`、注册服务）/`stop`/`free`；电源管理 `registerWithPolicyMaker`/`setPowerState`；数据输出 `outputPacket`/`outputStart`；`configureInterface` |
| `AirPortOpenBSD_Ioctl.cpp` | `apple80211Request` 请求分发器（switch 路由到各 get/set 处理函数）；约 40+ 个 IOCTL 处理函数（SSID/CHANNEL/SCAN_REQ/SCAN_RESULT/AUTH_TYPE/CIPHER_KEY/ASSOCIATE/RSSI/NOISE/PHY_MODE 等），将 Apple 802.11 语义映射到 OpenBSD `if_ioctl` |
| `AirPortOpenBSD_80211.cpp` | 802.11 原始帧与 BPF 输出接口实现（`outputRaw80211Packet`/`outputActionFrame`/`bpfOutputPacket`/`bpfOutput80211Radio`）；`useAppleRSNSupplicant` 返回 false（使用驱动自带 RSN） |
| `AirPortOpenBSD_Ethernet.cpp` | IOEthernet 形态的自动关联逻辑 `autoASSOC`，从 plist IFConfig 读取预配置网络并循环尝试关联 |
| `AirPortOpenBSD_Util.cpp` | 固件加载（`loadfirmware`/`firmwareLoadComplete`）；接收数据入队（`if_input`/`enqueueInputPacket2`/`flushInputQueue2`）；信道标志转换（`chanspec2applechannel`）；连接状态/扫描结果辅助函数 |
| `compat.cpp/.h` | OpenBSD 内核函数的 macOS 实现：`tsleep_nsec`/`wakeup_sleep`（基于 CommandGate）、`loadfirmware`、`if_start`/`if_input`/`if_link_state_change`、`ether_ifattach`/`ether_ifdetach`、中断分发 `interrupt_func` |
| `help_ifconfig.cpp/.h` | 内嵌 ifconfig 命令实现，支持 `nwid`/`wpakey`/`join`/`chan`/`bssid`/`nwkey`/`wpa` 等子命令；包含 `pkcs5_pbkdf2` 密钥派生与 `hmac_sha1` 实现 |
| `iwlwifi.h` | 驱动注册表：声明所有驱动的 `cfdriver`（设备ID表）与 `cfattach`（attach方法表），组装为 `cdlist`/`calist` 供 `probe` 遍历匹配 |

### 4.2 OpenBSD 协议栈（openbsd/net80211）

这是 OpenBSD 的 IEEE 802.11 协议栈，几乎原样移植，是驱动的"大脑"。

| 文件 | 职责 |
|------|------|
| `ieee80211_var.h` | 核心头文件：定义 `struct ieee80211com`（无线接口控制块，包含 `ic_if`/`ic_bss`/状态机/信道表/能力等）、PHY 模式/操作模式/保护模式枚举、信道结构 |
| `ieee80211.c` | 协议栈初始化、基本速率设置、AMPDU 配置、后台扫描（bgscan）等栈级逻辑 |
| `ieee80211_proto.c` | 协议状态机（SCAN→AUTH→ASSOC→RUN 等）、管理帧处理、认证/关联/去认证逻辑；定义状态名与物理模式名 |
| `ieee80211_node.c` | 节点（站点）管理：节点分配/释放/复制、BSS 切换、BA（块确认）请求、扫描结果缓存 |
| `ieee80211_input.c` | 接收路径：硬件解密、分片重组、A-MPDU 重排序、帧解封装、数据入队 |
| `ieee80211_output.c` | 发送路径：管理帧输出、A-MPDU 判断、RSN/WPA IE 添加、认证/去认证帧构造 |
| `ieee80211_ioctl.c/.h` | 802.11 ioctl 接口（SIOCG80211NWID 等），供 `if_ioctl` 调用 |
| `ieee80211_crypto.c/.h` | 加密框架：密钥安装/删除、按 cipher 分发到具体实现；TKIP MIC failure 超时处理 |
| `ieee80211_crypto_wep.c` | WEP 加密实现 |
| `ieee80211_crypto_tkip.c` | TKIP/CCMP 加密实现 |
| `ieee80211_crypto_ccmp.c` | CCMP（AES-CCM）加密实现 |
| `ieee80211_crypto_bip.c` | BIP（管理帧保护）实现 |
| `ieee80211_pae_input.c` | 802.1X/EAPOL 接收处理（WPA2 企业级认证） |
| `ieee80211_pae_output.c` | 802.1X/EAPOL 发送处理 |
| `ieee80211_amrr.c` | AMRR（自适应多速率重试）速率自适应算法 |
| `ieee80211_mira.c` | MiRA 速率自适应算法 |
| `ieee80211_rssadapt.c` | 基于 RSSI 的速率自适应算法 |
| `ieee80211_regdomain.c/.h` | 监管域（频段/功率法规）管理 |
| `ieee80211_radiotap.h` | Radiotap 头定义（接收信号元数据） |
| `ieee80211_priv.h` | 协议栈内部私有声明 |

### 4.3 OpenBSD 设备驱动（openbsd/dev）

| 驱动 | 文件 | 支持硬件 |
|------|------|----------|
| `iwm` | `dev/pci/if_iwm.c` + `if_iwmreg.h` + `if_iwmvar.h` | Intel 7260/7265/3160/3165/3168/8260/8265/9000/9260 系列（7000-9000 系列） |
| `iwx` | `dev/pci/if_iwx.c` + `if_iwxreg.h` + `if_iwxvar.h` | Intel Wi-Fi 6 AX200/AX201（Qu/Qnj/QuZ/cc 芯片） |
| `iwn` | `dev/pci/if_iwn.c` + `if_iwnreg.h` + `if_iwnvar.h` | Intel 4965/5100/5300/1000/6000/6050 系列 |
| `ipw` | `dev/pci/if_ipw.c` + `if_ipwreg.h` + `if_ipwvar.h` | Intel PRO/Wireless 2100/2200BG/2915ABG/3945ABG |
| `iwi` | `dev/pci/if_iwi.c` + `if_iwireg.h` + `if_iwivar.h` | Intel PRO/Wireless 2200BG/2915ABG |
| `wpi` | `dev/pci/if_wpi.c` + `if_wpireg.h` + `if_wpivar.h` | Intel PRO/Wireless 3945ABG |
| `bwfm` | `dev/ic/bwfm.c` + `dev/pci/if_bwfm_pci.c` | Broadcom FullMAC（brcmfmac4350/4356/43602/4371） |
| `bwi` | `dev/ic/bwi.c` + `dev/pci/if_bwi_pci.c` | Broadcom AirForce（bwi-airforce 固件） |
| `rtwn` | `dev/ic/rtwn.c` + `dev/pci/if_rtwn.c` | Realtek RTL8188EE/RTL8192CE/RTL8723BE |
| `pci.c` | `dev/pci/pci.c` | PCI 配置空间访问适配（`pci_conf_read`/`pci_conf_write`/`pci_mapreg_map`/`pci_matchbyid`），桥接 IOPCIDevice |
| `pcidevs.h` / `pcireg.h` / `pcivar.h` | `dev/pci/` | PCI 设备 ID 表与寄存器/变量定义 |

### 4.4 OpenBSD 内核兼容层（openbsd/sys, machine, netinet）

| 文件 | 职责 |
|------|------|
| `sys/_kernel.h` | 全局声明（`_ifp`/`logStr_i`）、调试宏（`DebugLog`/`DPRINTF`）、`err`/`warnx` 等用户态函数桩、包含所有 crypto 头 |
| `sys/kern_compat.h` | SPL 宏桩（`splnet`/`splx` 等）、`SET`/`CLR`/`ISSET`/`nitems` 宏 |
| `sys/device.h` | `struct device` 定义（含 `AirPortOpenBSD *dev` 反向指针、中断句柄数组、设备名 `dv_xname`） |
| `sys/mutex.h` | `struct mutex` 基于 `IORecursiveLock` 实现；`mtx_init`/`mtx_enter`/`mtx_leave` 宏 |
| `sys/rwlock.h` | 读写锁基于 IORWLock 适配 |
| `sys/task.h/.c` | 延迟任务（`struct task`）基于 IOCommandGate 实现 |
| `sys/timeout.h/.c` | `struct timeout` 超时机制基于 IOTimerEventSource 实现 |
| `sys/_mbuf.h` | mbuf/mbuf_list/mbuf_queue 兼容（适配 macOS `mbuf_t`） |
| `sys/_malloc.h/.c` | `malloc`/`free` 基于 `IOMalloc`/`IOFree`；`IONew`/`IODelete` 宏 |
| `sys/tree.h/.c` | 红黑树/RB 树数据结构 |
| `sys/refcnt.h` | 引用计数 |
| `sys/libkern.h` | 字符串/位操作库（`fls`/`strlcpy` 等） |
| `sys/timingsafe_bcmp.h/.c` | 常时比较 |
| `sys/__types.h` / `_null.h` / `endian.h` | 基本类型与字节序定义 |
| `machine/bus.h/.c` | 总线空间与 DMA 抽象：`bus_space_tag_t`=IOMemoryMap、`bus_dmamap` 基于 IODMACommand、内存映射/读写/屏障 |
| `machine/intr.h` | 中断抽象与 `pci_intr_handle` |
| `netinet/_if.h` | `struct if_rxring` 接收环管理 |
| `netinet/_if_ether.h` | **核心 `struct ifnet` 定义**（macOS-OpenBSD 桥梁），含 LINK_STATE 宏映射 |
| `netinet/_bpf.h/.c` | BPF（伯克利包过滤器）兼容 |
| `netinet/if_media.h` | 媒体类型定义（`IF_Mbps` 等） |

### 4.5 OpenBSD 加密库（openbsd/crypto）

为 net80211 加密（WEP/TKIP/CCMP/BIP）与 802.1X 认证提供密码学原语，全部为 OpenBSD 原始实现：

| 文件 | 算法 | 用途 |
|------|------|------|
| `aes.c/.h` | AES | CCMP/BIP 加密核心 |
| `sha1.c/.h` | SHA-1 | PRF/HMAC（WPA 密钥派生） |
| `sha2.c/.h` | SHA-256/384/512 | 高强度哈希 |
| `md5.c/.h` | MD-5 | 旧协议哈希 |
| `hmac.c/.h` | HMAC | 消息认证码 |
| `cmac.c/.h` | CMAC-AES | BIP 管理帧保护 |
| `arc4.c/.h` | RC4/ARC4 | WEP 加密 |
| `michael.c/.h` | Michael MIC | TKIP 完整性校验 |
| `key_wrap.c/.h` | AES Key Wrap | 802.1X 密钥封装 |

### 4.6 固件模块（firmware/）

| 文件 | 职责 |
|------|------|
| `firmware.h` | `struct firmware` 定义（name/data/size）、`FIRMWARE` 宏、`firmwares[]` 与 `firmwares_total` 声明 |
| `firmwarevar.h` | 由 `fw2bin.sh` 自动生成，声明所有固件数组的 extern |
| `firmware.c` | 由 `fw2bin.sh` 自动生成，定义 `firmwares[]` 数组（固件名→数据映射表） |
| `iwm_*.c` / `iwn_*.c` / `iwx_*.c` / `brcmfmac*.c` 等 | 各网卡固件的 C 数组（由 `xxd -i` 生成） |

### 4.7 Apple 802.11 框架头文件（apple80211/）

这些是 Apple 私有框架的逆向/提取头文件，定义 macOS 原生 Wi-Fi 子系统接口：

| 文件 | 职责 |
|------|------|
| `IO80211Controller.h` | `IO80211Controller` 基类（`apple80211Request` 虚函数、802.11 输出接口） |
| `IO80211Interface.h` | `IO80211Interface` 网络接口类 |
| `IO80211WorkLoop.h` | `IO80211WorkLoop` 专用 WorkLoop |
| `IOSkywalkEthernetInterface.h` | Skywalk 以太网接口（新版 macOS） |
| `apple80211_ioctl.h` | `apple80211req` 结构与 `SIOCSA80211`/`SIOCGA80211` 命令码、`APPLE80211_IOC_*` 请求号枚举 |
| `apple80211_var.h` | `apple80211_*_data` 数据结构定义（SSID/channel/scan/key/rssi 等约 60+ 结构） |
| `apple80211_wps.h` | WPS 支持 |

---

## 5. 关键类与函数说明

### 5.1 核心类 `AirPortOpenBSD`

继承自 `IOController`（宏定义，编译时展开为 `IO80211Controller` 或 `IOEthernetController`）。定义于 [AirPortOpenBSD.hpp](file:///workspace/AirPortOpenBSD/AirPortOpenBSD.hpp)。

#### 生命周期方法

| 方法 | 说明 |
|------|------|
| `init(OSDictionary*)` | 初始化，分配固件加载锁 `fwLoadLock` |
| `probe(IOService*, SInt32*)` | PCI 设备探测：读取 vendor/device ID，遍历 `calist` 调用各驱动 `ca_match` 匹配，禁用 MSI/MSIX |
| `start(IOService*)` | 核心启动：创建 WorkLoop/CommandGate/看门狗定时器；分配 `if_softc`（含 device + ieee80211com）；设置全局 `_ifp`；调用驱动 `ca_attach` 初始化硬件；发布 medium 字典；注册服务；Ethernet 形态下从 plist 读取 IFConfig 预填网络 |
| `stop(IOService*)` | 停止与资源释放 |
| `free()` | 最终释放 |

#### 数据收发方法

| 方法 | 说明 |
|------|------|
| `outputPacket(mbuf_t, void*)` | 数据包输出入口（从 IOKit 到驱动 TX） |
| `outputStart(IOInterface*, IOOptionBits)` | 启动输出队列 |
| `configureInterface(IONetworkInterface*)` | 配置网络接口参数 |
| `getHardwareAddress(IOEthernetAddress*)` | 获取 MAC 地址 |
| `setPromiscuousMode` / `setMulticastMode` / `setMulticastList` | 混杂/组播模式设置 |
| `enqueueInputPacket2(mbuf_t)` | 接收数据入队（驱动 RX → IOKit） |
| `flushInputQueue2()` | 刷新接收队列到上层 |
| `if_input(ifnet*, mbuf_list*)` | OpenBSD 风格的接收入队（解封装后调用） |

#### apple80211 IOCTL 方法

| 方法 | 说明 |
|------|------|
| `apple80211Request(type, number, interface, data)` | **核心分发器**：校验请求类型后按 `request_number` switch 路由到对应 get/set 处理函数 |
| `getSSID` / `setSSID` | SSID 读写（set 通过内嵌 ifconfig 设置 nwid） |
| `getCHANNEL` / `setCHANNEL` | 信道读写 |
| `setSCAN_REQ` / `setSCAN_REQ_MULTIPLE` | 触发扫描 |
| `getSCAN_RESULT` | 获取扫描结果（将 OpenBSD `ieee80211_nodereq` 转为 `apple80211_scan_result`） |
| `setASSOCIATE` | 关联到 AP（核心连接入口） |
| `setDISASSOCIATE` | 断开关联 |
| `setCIPHER_KEY` | 安装密钥（802.1X 时通过 `SIOCS80211KEYAVAIL` 传给驱动） |
| `getRSSI` / `getNOISE` / `getRATE` | 信号/噪声/速率查询 |
| `getPHY_MODE` / `getOP_MODE` | 物理/操作模式 |
| `getCARD_CAPABILITIES` | 网卡能力 |
| `getSUPPORTED_CHANNELS` | 支持信道列表 |
| `setPOWER` / `getPOWER` | 电源开关 |
| `getCOUNTRY_CODE` / `setCOUNTRY_CODE` | 国家/地区代码 |
| `getDRIVER_VERSION` / `getHARDWARE_VERSION` | 版本信息 |
| `setSCANCACHE_CLEAR` | 清除扫描缓存 |

#### 电源管理

| 方法 | 说明 |
|------|------|
| `registerWithPolicyMaker(IOService*)` | 向电源策略注册器注册电源状态 |
| `setPowerState(unsigned long, IOService*)` | 切换电源状态（开/关 radio） |
| `setPowerStateAction(...)` | 静态回调，通过 CommandGate 执行 `setPowerState` |

#### 固件与扫描工具

| 方法 | 说明 |
|------|------|
| `loadfirmware(const char*, u_char**, size_t*)` | 从内嵌 `firmwares[]` 表查找并加载固件到内存 |
| `firmwareLoadComplete(const char*)` | 固件加载完成回调，匹配固件表填充 `firmwareData` |
| `scanDone(OSObject*, ...)` | 扫描完成回调 |
| `scanConvertResult(ieee80211_nodereq*, apple80211_scan_result*)` | OpenBSD 扫描结果→Apple 格式转换 |
| `findScanResult(apple80211_assoc_data*)` | 按关联请求查找匹配的扫描结果 |
| `chanspec2applechannel(int flags)` | OpenBSD 信道标志→Apple 信道标志转换 |
| `setLinkState(int)` | 设置链路状态并通知上层 |
| `autoASSOC(OSObject*, ...)` | Ethernet 形态自动关联定时回调 |

### 5.2 兼容层关键函数（compat.cpp）

| 函数 | 说明 |
|------|------|
| `tsleep_nsec(ident, pri, wmesg, timo)` | OpenBSD 内核睡眠语义→macOS `IOCommandGate::runAction` + `commandWakeup`，是驱动同步等待固件/命令完成的核心 |
| `wakeup_sleep(ident, one)` | 唤醒在 `ident` 上睡眠的线程 |
| `interrupt_func(owner, src, count)` | 中断分发：从 `pci_intr_handle` 取出驱动中断回调并调用 |
| `loadfirmware(name, bufp, buflen)` | 包装全局 `_ifp` 调用 `AirPortOpenBSD::loadfirmware` |
| `if_start(ifp)` / `if_input(ifp, ml)` | 通过 `if_softc→dev` 反向调用 `AirPortOpenBSD` 方法 |
| `ether_ifattach(ifp)` / `ether_ifdetach(ifp)` | 以太网接口 attach/detach |
| `if_link_state_change(ifp)` | 链路状态变更通知 |

### 5.3 ifconfig 实现（help_ifconfig.cpp）

| 函数 | 说明 |
|------|------|
| `ifconfig(const char**, int)` / `ifconfig(const char*)` | 内嵌 ifconfig 命令解析与执行入口 |
| `setifnwid(argv, argc)` | 设置网络名（SSID） |
| `setifwpakey(argv, argc)` | 设置 WPA 密钥 |
| `setifjoin(argv, argc)` / `delifjoin(argv, argc)` | 加入/离开网络 |
| `setifbssid` / `setifchan` / `setifnwkey` / `setifwpa*` | BSSID/信道/WEP密钥/WPA参数设置 |
| `pkcs5_pbkdf2(pass, salt, key, rounds)` | PBKDF2 密钥派生（WPA-PSK 密钥生成） |
| `hmac_sha1(text, key, digest)` | HMAC-SHA1 实现 |
| `process_join_commands()` | 处理加入命令队列 |

### 5.4 PCI 框架适配（dev/pci/pci.c）

| 函数 | 说明 |
|------|------|
| `pci_conf_read` / `pci_conf_write` | PCI 配置空间读写（委托 `IOPCIDevice::configRead32`/`configWrite32`） |
| `pci_mapreg_map(pa, reg, ...)` | 映射 PCI BAR 寄存器（`IOPCIDevice::mapDeviceMemoryWithRegister`） |
| `pci_matchbyid(pa, ids, nids)` | 按 vendor/device ID 匹配设备 |
| `pci_disable_msi` / `pci_disable_msix` | 禁用 MSI/MSIX（使用传统 INTx 中断） |

---

## 6. 核心数据结构

### 6.1 `struct ifnet`（macOS-OpenBSD 桥梁）

定义于 [openbsd/netinet/_if_ether.h](file:///workspace/AirPortOpenBSD/openbsd/netinet/_if_ether.h)，是整个移植的关键。它在 OpenBSD 原始 `ifnet` 基础上扩展了 IOKit 字段：

```c
struct ifnet {
    void *if_softc;                 // 指向 device + ieee80211com（驱动私有数据）
    struct refcnt if_refcnt;
    char if_xname[IFNAMSIZ];        // 接口名
    unsigned short if_flags;        // up/down, broadcast 等标志
    struct if_data if_data;         // 统计数据
    /* OpenBSD 函数指针（驱动填充） */
    int (*if_output)(...);          // 输出例程
    int (*if_start)(struct ifnet*); // 发起输出
    int (*if_ioctl)(struct ifnet*, u_long, caddr_t); // ioctl 钩子
    void (*if_watchdog)(struct ifnet*);              // 看门狗
    struct mbuf_queue if_snd;       // 发送队列
    /* ===== macOS IOKit 扩展字段 ===== */
    IONetworkStats *netStat;        // 网络统计（if_opackets 等宏映射到此）
    IOInterface *iface;             // IOKit 网络接口对象
    IOWorkLoop *fWorkloop;          // IOKit WorkLoop
    IOCommandGate *fCommandGate;    // IOKit 命令门（同步原语）
    int if_link_state;              // 链路状态
    int if_power_state;             // 电源状态
    char fwname[256];               // 固件名缓存
};
```

全局变量 `extern struct ifnet *_ifp` 指向当前活动接口，是 OpenBSD 代码访问 macOS 适配层的统一入口。

### 6.2 `struct ieee80211com`（无线控制块）

定义于 `ieee80211_var.h`，包含无线协议栈全部状态：

- `ic_if`：内嵌的 `ifnet`
- `ic_bss`：当前关联的 BSS 节点
- `ic_state`：协议状态机状态（SCAN/AUTH/ASSOC/RUN）
- `ic_channels[]`：支持的信道表
- `ic_caps`：能力标志
- 各类回调与节点表

### 6.3 `struct device`（设备抽象）

定义于 [openbsd/sys/device.h](file:///workspace/AirPortOpenBSD/openbsd/sys/device.h)：

```c
struct device {
    AirPortOpenBSD* dev;            // 反向指针，指向 kext 主对象
    pci_intr_handle *ih[PCI_MSIX_QUEUES]; // 中断句柄
    u_int ih_count;
    char dv_xname[16];             // 设备名
};
```

`if_softc` 内存布局为 `[struct device | struct ieee80211com]`，驱动通过 `if_softc` 访问两者。

### 6.4 `struct firmware`（固件表项）

```c
struct firmware {
    const char *name;
    const unsigned char *data;
    const unsigned int size;
};
extern const struct firmware firmwares[];  // 全部固件映射表
extern const int firmwares_total;
```

### 6.5 驱动注册表（iwlwifi.h）

```c
// cfdriver 列表（设备 ID 匹配表）
static struct cfdriverlist cdlist = {
    {iwx_cd, iwm_cd, iwn_cd, wpi_cd, iwi_cd, ipw_cd, rtwn_cd, NULL, NULL},
    DRIVER_COUNT  // = 9
};
// cfattach 列表（attach 方法表）
static struct cfattachlist calist = {
    {iwx_ca, iwm_ca, iwn_ca, wpi_ca, iwi_ca, ipw_ca, rtwn_pci_ca, bwfm_pci_ca, bwi_pci_ca},
    DRIVER_COUNT
};
```

`probe()` 遍历 `calist`，调用每个驱动的 `ca_match` 进行设备匹配，匹配成功后 `start()` 调用对应 `ca_attach` 初始化硬件。

### 6.6 `apple80211req`（Apple IOCTL 请求）

```c
struct apple80211req {
    char req_if_name[IFNAMSIZ]; // 接口名
    int  req_type;               // SIOCSA80211(set) / SIOCGA80211(get)
    int  req_val;                // 子值
    u_int32_t req_len;          // 数据长度
    void *req_data;              // 指向 apple80211_*_data 结构
};
```

请求号 `APPLE80211_IOC_*`（如 SSID=1, AUTH_TYPE=2, CIPHER_KEY=3, CHANNEL=4, SCAN_REQ=10, SCAN_RESULT=11, ASSOCIATE=20 等）定义于 `apple80211_ioctl.h`。

---

## 7. 依赖关系

### 7.1 macOS 内核库依赖（Info.plist OSBundleLibraries）

| 依赖 | 版本 | 用途 |
|------|------|------|
| `com.apple.iokit.IO80211Family` | 600.0 | 原生 Wi-Fi 框架（仅 IO80211 形态） |
| `com.apple.iokit.IONetworkingFamily` | 3.1 | 网络控制器/接口基类 |
| `com.apple.iokit.IOPCIFamily` | 2.8 | PCI 设备访问 |
| `com.apple.kpi.bsd` | 13.0.0 | BSD 子系统（mbuf/socket 等） |
| `com.apple.kpi.iokit` | 13.0.0 | IOKit 核心 |
| `com.apple.kpi.libkern` | 13.0.0 | 内核基础库（OSObject 等） |
| `com.apple.kpi.mach` | 13.0.0 | Mach 内核原语 |
| `com.apple.kpi.unsupported` | 13.0.0 | 非公开 KPI（仅 IO80211 形态） |

> `OSBundleRequired` = `Network-Root`，表明该 kext 是网络启动所必需。

### 7.2 内部模块依赖图

```
AirPortOpenBSD (主类)
    ├── apple80211/ (Apple 框架头) ── 提供 IO80211Controller/IO80211Interface 基类与 IOCTL 定义
    ├── firmware/ ────────────────── 提供内嵌固件数据
    │     └── sources/firmware/ ──── fw2bin.sh 原始输入
    ├── openbsd/net80211/ ────────── 协议栈（被驱动调用，调用 sys/）
    ├── openbsd/dev/pci/ ─────────── 设备驱动（调用 net80211、machine/bus、sys/）
    │     └── openbsd/dev/ic/ ────── IC 级驱动辅助
    ├── openbsd/machine/ ─────────── 总线/DMA 抽象（驱动调用）
    ├── openbsd/sys/ ─────────────── 内核原语兼容（被所有 OpenBSD 代码调用）
    ├── openbsd/netinet/ ─────────── ifnet/BPF/媒体类型（协议栈与驱动依赖）
    ├── openbsd/crypto/ ─────────── 加密算法（net80211_crypto* 调用）
    └── compat.cpp ───────────────── 桥接 OpenBSD 函数调用到 AirPortOpenBSD 方法
```

### 7.3 编译依赖链

`_kernel.h` → `kern_compat.h` + `libkern.h` + 所有 `crypto/*.h` → `net80211/*` → `dev/pci/*` → `AirPortOpenBSD.hpp` → `AirPortOpenBSD*.cpp`

---

## 8. 项目运行与构建方式

### 8.1 构建 Target

项目使用 Xcode 构建，`project.pbxproj` 中定义了以下 Target（均产出 `AirPortOpenBSD.kext`）：

| Target | 形态 | 适用系统 | 说明 |
|--------|------|----------|------|
| `AirPortOpenBSD_BigSur` | IO80211 | macOS 11 Big Sur | 使用 `BIG_SUR` 宏，DebugLog 写入属性 |
| `AirPortOpenBSD_Catalina` | IO80211 | macOS 10.15 Catalina | 使用 `CATALINA` 宏（ioctl 码硬编码） |
| `AirPortOpenBSD_Mojave` | IO80211 | macOS 10.14 Mojave | — |
| `AirPortOpenBSD_HighSierra` | IO80211 | macOS 10.13 High Sierra | — |
| `AirPortOpenBSD_Ethernet` | IOEthernet | 通用 | 不依赖 IO80211Family，通过 plist IFConfig 配置网络，支持旧系统 |
| `fw2bin` | 命令行工具 | — | 将 `sources/firmware/*.bin` 转为 `firmware/*.c` C 数组 |

### 8.2 固件生成流程

使用 [fw2bin.sh](file:///workspace/fw2bin.sh) 脚本（或 `fw2bin` target）将原始固件二进制转为可编译的 C 数组：

```sh
# 输入: AirPortOpenBSD/sources/firmware/*.bin
# 输出:
#   AirPortOpenBSD/firmware/<name>.c       (每个固件一个文件，含 xxd -i 生成的数组)
#   AirPortOpenBSD/firmware/firmwarevar.h  (所有数组的 extern 声明)
#   AirPortOpenBSD/firmware/firmware.c     (firmwares[] 映射表 + firmwares_total)
```

### 8.3 构建步骤

1. 使用 Xcode 打开 `AirPortOpenBSD.xcodeproj`
2. 选择对应系统的 Scheme（如 `AirPortOpenBSD_BigSur`）
3. Build（Debug 用于调试，Release 用于发布）
4. 产出 `AirPortOpenBSD.kext`

### 8.4 安装与加载

#### 方式一：标准加载（手动测试）

```sh
# 复制到系统扩展目录
sudo cp -R AirPortOpenBSD.kext /Library/Extensions/

# 加载 kext
sudo kextload /Library/Extensions/AirPortOpenBSD.kext

# 查看加载状态
kextstat | grep AirPortOpenBSD

# 卸载
sudo kextunload -b com.zhong.AirPortOpenBSD
```

#### 方式二：开机加载（推荐，配合 OpenCore）

按 VERSION 中 v1.3.1 的说明：
1. 将驱动放置于 `IO80211Family.kext/Contents/PlugIns/AirPortOpenBSD.kext`
2. 将 `IO80211Family.kext` 加入 OpenCore 驱动目录（EFI/OC/Kexts）
3. 修改 `config.plist` 添加该 kext 条目

#### 方式三：NVRAM 配置（Ethernet 形态网络切换）

```sh
sudo nvram "WPA/WPA2"=true
sudo nvram NWID=FAST_88FED0
sudo nvram WPAKEY=126abc\!@ABC
# 开放式网络:
sudo nvram "WPA/WPA2"=false
sudo nvram NWID=Starbucks
```

关闭再打开 Wi-Fi 即可切换网络。NWID 为空时回退到 Info.plist 的 IFConfig 配置。

### 8.5 PCI 设备匹配

`Info.plist` 中 `IOPCIPrimaryMatch` = `0x00008086&0x0000ffff`，匹配所有 vendor=0x8086（Intel）的 PCI 设备。实际细分匹配由各驱动的 `ca_match` 函数（基于 `pcidevs.h` 中的设备 ID 表）完成。

---

## 9. 核心工作流程

### 9.1 驱动加载与初始化流程

```
IOKit 匹配 IOPCIDevice
    │
    ▼
probe(): 读取 vendor/device ID → 遍历 calist → ca_match() 匹配驱动
    │                                            → 禁用 MSI/MSIX
    ▼
start():
    ├── 创建 WorkLoop / CommandGate / WatchdogTimer / TimerEventSource
    ├── malloc if_softc (device + ieee80211com)
    ├── _ifp = &ic->ic_if; 设置 fWorkloop/fCommandGate
    ├── 启用 PCI 内存/IO/BusMaster
    ├── ca_attach() ──→ 驱动初始化硬件、加载固件、注册中断
    │       └── loadfirmware() ──→ 从内嵌 firmwares[] 查找
    ├── 发布 medium 字典
    └── registerService() ──→ IOKit 发布网络接口
```

### 9.2 Wi-Fi 关联流程

```
macOS Wi-Fi 客户端 (AirportUtility)
    │ SIOCSA80211 (set)
    ▼
apple80211Request(SET, ASSOCIATE, ...)
    │
    ▼
setASSOCIATE():
    ├── setSCAN_REQ() ──→ 触发 net80211 扫描
    │       └── ieee80211 扫描状态机 → 驱动切换信道/捕获 beacons
    ├── scanDone() ──→ scanConvertResult() 转换结果到 apple80211 格式
    ├── findScanResult() ──→ 匹配目标 AP
    └── ifconfig("join SSID wpakey ...")
            ├── setifnwid() ──→ if_ioctl(SIOCS80211NWID)
            ├── pkcs5_pbkdf2() ──→ 派生 WPA 密钥
            └── setifwpakey() ──→ 安装密钥
    │
    ▼
net80211 协议状态机: SCAN → AUTH → ASSOC → RUN
    │
    ▼
setLinkState(LINK_STATE_UP) ──→ 通知 macOS 链路就绪 → DHCP
```

### 9.3 数据收发流程

**发送（macOS → 硬件）：**
```
上层协议栈 outputPacket(mbuf)
    → outputStart() / outputPacket()
        → if_start(ifp) ──→ 驱动 if_start 回调
            → 驱动 TX 描述符填充 → DMA → 网卡发送
```

**接收（硬件 → macOS）：**
```
网卡 RX 中断 → interrupt_func() → 驱动中断处理
    → 驱动从 RX 描述符取包 → ieee80211_input() 解密/解封装
        → if_input(ifp, mbuf_list) ──→ AirPortOpenBSD::if_input()
            → enqueueInputPacket2(m) ──→ flushInputQueue2()
                → IOKit 上交协议栈
```

### 9.4 apple80211 IOCTL 处理流程

```
用户态 ioctl(SIOCGA80211/SIOCSA80211, apple80211req)
    │
    ▼
AirPortOpenBSD::apple80211Request(request_type, request_number, interface, data)
    │ 校验 request_type ∈ {SIOCGA80211, SIOCSA80211}
    ▼
switch (request_number)  // APPLE80211_IOC_*
    ├── case SSID:        → getSSID()/setSSID()
    ├── case SCAN_REQ:    → setSCAN_REQ()
    ├── case SCAN_RESULT: → getSCAN_RESULT()
    ├── case ASSOCIATE:   → setASSOCIATE()
    ├── case RSSI:        → getRSSI()
    └── ... (40+ 分支)
```

各 get/set 函数内部通常通过 `_ifp->if_ioctl(_ifp, SIOC*, ...)` 调用 OpenBSD 驱动的 ioctl 处理，或直接访问缓存的 `apple80211_*_data` 成员。

---

## 10. 版本演进与支持硬件

### 10.1 当前版本

v2.0.7（见 [VERSION](file:///workspace/VERSION)）

主要更新：
- 更新 2021/01/17 版 OpenBSD iwx 等驱动
- 优化 Wi-Fi 列表
- 优化唤醒后不自动连接
- 优化内存分配与释放

### 10.2 支持 Intel Wi-Fi 硬件 ID 摘录

| 系列 | 设备 ID 示例 | 说明 |
|------|-------------|------|
| Wi-Fi 6 AX200 | `0x2723` | iwx 驱动 |
| Wi-Fi 6 AX201 | `0x02f0`（子ID 0x0070/0074/0078/007c/0310/2074/4070） | iwx 驱动 |
| 9000 系列 | `0x2526`/`0x9df0`/`0xa370` | iwm 驱动 |
| 8000 系列 | `0x24f3`/`0x24f4`/`0x24fd` | iwm 驱动 |
| 7000 系列 | `0x3165`/`0x3166`/`0x08b1`/`0x08b2`/`0x095a`/`0x095b` | iwm 驱动 |
| 3160 系列 | `0x08b3`/`0x08b4`/`0x24fb` | iwm 驱动 |
| 4965 | `0x4229`/`0x4230` | iwn 驱动 |
| 5100/5300 | `0x4232`/`0x4235` | iwn 驱动 |
| 1000~6050 | `0x0083`/`0x0087`/`0x0090` 等 | iwn 驱动 |
| 3945ABG | `0x4222`/`0x4227` | wpi 驱动 |
| 2200BG/2915ABG | `0x4220`/`0x4223` | ipw/iwi 驱动 |

### 10.3 认证方式支持

| 认证方式 | 支持 | 说明 |
|----------|------|------|
| OPEN | ✅ | 开放网络 |
| WEP | ✅ | 有线等效加密 |
| WPA-PSK | ✅ | WPA 个人 |
| WPA2-PSK | ✅ | WPA2 个人（主流） |
| WPA2-Enterprise | ✅ | 802.1X 企业级（校园网） |

### 10.4 已知限制

- 切换 Wi-Fi 可能触发内核恐慌（历史问题）
- 网络切换后 IP 可能不更新，需手动"DHCP 续租"
- 除 `iwm`/`iwx` 外其他驱动有待验证
- 部分加密协议（TKIP）发送数据曾有问题，后续版本已修复

---

## 附录：关键宏与编译开关

| 宏 | 作用 |
|----|------|
| `Ethernet` | 启用 IOEthernet 形态（`IOController`→`IOEthernetController`） |
| `BIG_SUR` | Big Sur 适配（DebugLog 写入设备属性） |
| `CATALINA` | Catalina 适配（ioctl 码硬编码） |
| `IEEE80211_DEBUG` / `IWM_DEBUG` | 启用调试日志 |
| `IEEE80211_STA_ONLY` | 仅 STA 模式（去除 AP/IBSS） |
| `INET6` | IPv6 支持 |
| `NBPFILTER` | BPF 包过滤支持 |
| `DEBUG` | 启用 DebugLog 宏 |

---

*本文档基于 AirPortOpenBSD v2.0.7 源码分析生成。*
