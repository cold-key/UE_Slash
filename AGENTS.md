# 仓库协作规范

## 项目结构与模块组织

`Slash.uproject` 定义 Unreal Engine 项目及其 `Slash` 运行时模块。

- `Source/Slash/Public/` 存放头文件，`Private/` 存放实现，按 `Characters`、`Pawns`、`Items` 等功能组织。
- `Source/Slash.Build.cs` 声明模块依赖；`Source/*.Target.cs` 定义游戏和编辑器构建目标。
- `Content/` 存放地图、蓝图、纹理、特效及导入的资源包。启动地图为 `Content/Maps/SlashOpenWorld.umap`。
- `SourceArt/` 存放纹理、音频等源资源；`Config/` 存放项目默认配置和输入映射。
- `Plugins/VisualStudioTools/` 存放 Visual Studio 集成代码。
- `Binaries/`、`Intermediate/`、`DerivedDataCache/`、`Saved/` 为生成目录，不纳入版本控制。

## 构建与开发命令

使用 `Slash.uproject` 关联的引擎，其关联标识为本机 GUID。安装包含 Unreal C++ 开发工具的 Visual Studio。在仓库根目录将 `$env:UE_ROOT` 设置为对应引擎目录，不将本机绝对路径写入共享配置。

每次启动可能调用构建工具的命令前，先在同一命令环境中设置以下变量。后续工具调用若使用新的命令环境，需要重新设置。

```powershell
# 禁止复用构建工作进程，避免受限进程影响后续构建。
$env:MSBUILDDISABLENODEREUSE = '1'
$env:DOTNET_CLI_USE_MSBUILD_SERVER = '0'

# 生成 Visual Studio 项目文件。
& "$env:UE_ROOT/Engine/Build/BatchFiles/GenerateProjectFiles.bat" "-project=$PWD/Slash.uproject" -game -engine

# 构建 Windows 编辑器目标。
& "$env:UE_ROOT/Engine/Build/BatchFiles/Build.bat" SlashEditor Win64 Development "-Project=$PWD/Slash.uproject" -WaitMutex

# 启动编辑器，通过编辑器内运行验证玩法。
& "$env:UE_ROOT/Engine/Binaries/Win64/UnrealEditor.exe" "$PWD/Slash.uproject"
```

按任务需要选择命令，不要求每次执行全部命令。

## UE 构建与进程管理

- 启动任何可能调用 UnrealBuildTool、dotnet 或 MSBuild 的命令前，必须设置上述两个环境变量，包括构建、生成项目文件和 UnrealEditor-Cmd 自动化命令。
- 直接调用 MSBuild 时使用 `/nodeReuse:false`。
- 环境变量仅作用于当前命令及其子进程，不修改用户或系统环境变量。
- 源码引擎构建会写入引擎目录。引擎目录不在沙箱可写范围时，应通过权限机制执行，避免先在受限环境中启动构建。沙箱外执行不等同于以管理员身份运行。
- 任务结束时检查本任务启动的后台构建进程；仅清理能够确认属于本任务、已经空闲且父进程已退出的残留进程。
- 禁止批量终止所有 dotnet、MSBuild、Visual Studio 或 UE 进程，不干扰用户正在进行的构建和编辑。
- 遇到 `Access denied` 时，先检查失败路径、文件属性、访问权限及构建进程的受限令牌，不直接认定需要管理员权限，不擅自修改目录 ACL。

## Visual Studio 项目同步

- 新增、删除或移动 C++ 文件后，在文件变更完成时统一生成一次 Visual Studio 项目文件，再进行必要的构建。
- 使用 `Slash.uproject` 关联的引擎生成项目文件。
- 生成成功后检查对应 `.vcxproj` 和 `.vcxproj.filters`，确认新增文件已包含，并提醒用户重新加载解决方案。
- 不手工维护 UE 自动生成的项目文件。

## 编码风格与命名约定

- 遵循周边 C++ 代码风格，使用制表符缩进，保持头文件与源文件名称对应。
- 使用 Unreal 类型前缀，例如 `ASlashCharacter`、`UCameraComponent`、`FVector`。
- 头文件中的生成头文件包含项必须放在最后。
- 保留反射属性名称；需要改名时，同步迁移蓝图引用。
- 新增模块依赖在 `Slash.Build.cs` 中声明。
- 当前未配置全项目格式化或静态检查工具。

## 验证要求

- 修改 C++ 逻辑或玩法后，构建 `SlashEditor`，并在 `SlashOpenWorld` 中验证受影响的移动、相机、跳跃、Pawn、物品或动画行为，检查输出日志中的错误。
- 无法完成编辑器内运行或目视检查时，明确报告已执行的验证及尚未验证的内容，不将无渲染测试等同于视觉验收。
- 自动化测试位于 `Source/Slash/Private/Tests/`，命名为 `Slash.<Feature>.<Behavior>`，可通过 Session Frontend 的 Automation 页运行。当前已有 `Slash.Animation` 相关测试，未设置覆盖率门槛。
- 按改动风险和仓库要求选择充分的验证；验证通过后，仅在新增修改、失败或具体风险出现时追加验证。
- 纯文档修改无需构建 UE 项目，检查内容和差异即可。

## 执行效率与任务范围

- 优先完成用户指定的最小完整改动，不默认扩展为通用工具、额外文档或大规模测试建设。
- 日志写入 `Saved/`，默认只读取末尾和首个关键错误附近的内容，避免输出整份大型日志。
- 同一错误不原样反复重试，先取得新证据再调整方案。
- 不默认清空 `Intermediate/`、引擎缓存或强制重建 UnrealBuildTool；仅在诊断证据支持时执行针对性的修复。

## Unreal Engine 上下文读取策略

- C++ 逻辑优先通过 `Source/` 目录中的符号搜索和局部源码阅读定位。
- 不将 `.uasset`、`.umap` 当作普通文本源码读取。
- 需要理解蓝图逻辑时，优先使用 Unreal MCP。
- Unreal MCP 不可用或不支持所需查询时，先说明限制，再使用 Unreal Editor 界面或编辑器提供的自动化接口进行针对性检查，仍遵守以下范围限制。
- 只读取与当前任务直接相关的资产、图表、事件和函数。
- 不默认枚举全部 `Content/` 资产或导出完整蓝图；只有局部信息不足且有明确诊断需要时，才逐步扩大范围。
- 蓝图分析优先输出执行链、条件分支、函数调用和关键变量读写，不输出无关引脚及节点元数据。
- 对复杂蓝图按事件图、函数和宏分阶段分析。
- 发现 C++ 与蓝图调用关系后，记录对应类、函数和资产路径。
- 完成分析后，将已验证且可复用的架构结论写入项目文档，优先更新 `Docs/` 中的现有文档；不为简单查询额外创建文档，也不记录大量临时调试细节。
- 后续任务优先复用已有架构文档，并对与当前改动有关的结论做局部核实，避免重复扫描全部资产。

## 提交与合并请求规范

- 提交标题使用简洁的祈使句，清楚说明改动目的，例如“修复鸟类相机旋转”。
- 保持改动聚焦，不混入无关的重构、配置或资源变更。
- 合并请求应说明行为变化、受影响的地图或资源及验证结果；适用时关联问题，涉及视觉变化时提供截图或视频。

## 资源与配置管理

- 通过 Unreal Editor 编辑 `.uasset` 和 `.umap` 文件，包括使用编辑器提供的自动化接口，不直接修改二进制内容。
- 保存 World Partition 地图时，一并检查相关外部 Actor 和 Object 文件的变化。
- 仔细审查 `Config/` 变更，不将生成的构建输出提交到版本控制。
