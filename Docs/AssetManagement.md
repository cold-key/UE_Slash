# Unreal 资源目录管理

外部资源包统一放在 `Content/Assets/<资源包名>/`，保留资源包内部的目录结构。项目自己的地图、蓝图和效果继续放在对应的项目目录中。

## 下次移动资源

1. 保存全部资源，并提交 Git 或备份 `Content`、`Config` 和 `.uproject`。先确认资源包需要的插件可用，资源能够正常打开。
2. 在 Unreal Editor 的 Content Browser 中，将一个资源包拖到 `Content/Assets`，选择 **Move Here**。每次处理一个资源包；地图也在编辑器中移动。
3. 保存全部。检查 Output Log 和保存结果，确认没有 `Failed to save`、`Illegal reference`、缺少类或插件等错误，再继续移动下一包。
4. 右键 Content 根目录，执行 **Update Redirector References** 或 **Fix Up Redirectors in Folder**（菜单名称随引擎版本变化），然后再次保存。
5. 重启编辑器，打开相关地图和蓝图，运行 PIE，检查角色、动画、材质和日志。World Partition 地图保存时，连同相关外部 Actor/Object 文件一起检查。
6. 检查 Git 差异：新路径应新增，旧路径应删除，引用这些资源的蓝图和地图可能更新。确认代码和配置中的硬编码路径也已更新，然后提交。

日常移动在编辑器内完成。需要通过后台 Unreal 进程批量修复时，先关闭交互式编辑器，避免两边同时保存资源。

## 旧目录还在时

- 在 Content Browser 的筛选器中显示 Redirectors。重定向器负责将旧路径转到新路径，可通过上面的引用更新操作清理。
- 如果里面仍有真实网格、材质或贴图，检查保存错误和 Reference Viewer。真实重复资源需要核对版本、替换引用后再合并；只清理重定向器无法解决。
- 不要在资源管理器中直接移动或删除 `.uasset`、`.umap`，也不要对保存失败的资源反复强制重试。
- 如果提示 `Illegal reference to private object`，记录报错中的资源、对象路径和属性。比如 `ThumbnailInfo` 指向另一个包的私有缩略图对象，需要修复该引用后保存。
- 插件模块与源码引擎版本不匹配时，通过正常构建解决，不要手工修改二进制的 BuildId 来跳过检查。

本次修复前的备份和诊断记录放在 `Saved/AssetReorganization/`，该目录不纳入 Git。
