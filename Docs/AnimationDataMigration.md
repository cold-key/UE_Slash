# 将动画数据准备移到 C++

这次改造将 `ABP_SlashCharacter` 的父类从 `UAnimInstance` 改为 `USlashAnimInstance`。C++ 每帧准备角色数据，原来的 AnimGraph、状态机、动画混合和脚部 IK 继续使用这些数据。

## 1. 从新增类看职责

- `Source/Slash/Public/Characters/SlashAnimInstance.h`：声明动画参数及生命周期方法。
- `Source/Slash/Private/Characters/SlashAnimInstance.cpp`：获取角色并更新参数。

继承关系是：

```text
UAnimInstance
    └── USlashAnimInstance             C++：准备数据
            └── ABP_SlashCharacter     蓝图：组织和计算动画姿势
```

角色 Mesh 的 Anim Class 仍然是 `ABP_SlashCharacter`。不要把它改成裸的 `SlashAnimInstance`，后者没有角色的动画图。

## 2. 理解四个属性

| 属性 | C++ 类型 | 数据来源 |
| --- | --- | --- |
| `Speed` | `double` | `CharacterMovement->Velocity.Size2D()` |
| `isFalling` | `bool` | `CharacterMovement->IsFalling()` |
| `EquipState` | `ESlashCharacterEquipState` | 角色的 `GetEquipState()` |
| `AttackState` | `ESlashCharacteAttackState` | 角色的 `GetAttackState()` |

原蓝图中的 `Speed` 是双精度浮点数，这里保留 `double`，避免改变动画图的引脚类型。`Size2D()` 只取水平速度，跳跃时的垂直速度不会让地面移动动画加速。

`BlueprintReadOnly` 让动画蓝图读取参数，但不能直接对这些属性赋值；`Transient` 表示这些运行时数据不作为资产状态保存。状态的来源仍然是角色，AnimInstance 保存供动画图使用的当前快照。

另外一个属性 `SlashCharacter` 是 `ASlashCharacter` 引用，用于缓存当前拥有者，并供现有攻击结束通知访问角色。它不再要求拥有者必须是某个具体角色蓝图。

## 3. 理解生命周期

1. `NativeInitializeAnimation()`：动画实例初始化时读取当前拥有者和数据。
2. `NativeUpdateAnimation()`：每次动画更新时刷新四个参数。拥有者变化或引用失效时重新获取角色。
3. `NativeUninitializeAnimation()`：动画实例解除初始化时清理角色引用并重置参数。

没有角色拥有者的编辑器预览也会更新动画，因此不能无条件访问角色。当前实现会返回零速度、非下落、未装备、未攻击的默认值。

这次只在游戏线程的原生更新中采集数据，没有把 Actor 或 MovementComponent 的读取放进 `NativeThreadSafeUpdateAnimation()`。将来需要线程优化时，应先设计数据快照，再将合适的纯计算移到工作线程。

## 4. 对照编辑器检查蓝图迁移

这些修改已经通过 Unreal Editor 的图编辑 API 完成，可以在编辑器中逐项对照：

1. 打开 `Content/Blueprints/Characters/ABP_SlashCharacter`，在 Class Settings 中确认 Parent Class 是 `SlashAnimInstance`。
2. 在 My Blueprint 面板启用显示继承变量，查看 C++ 提供的四个动画参数和 `SlashCharacter`。
3. 原蓝图自己的六个变量已删除：`BP_SlashCharacter`、`Character Movement`、`Speed`、`isFalling`、`EquipState`、`AttackState`。四个动画参数现在来自父类。
4. Event Graph 中已移除初始化、转换具体角色蓝图、缓存 MovementComponent、逐帧赋值这两条链。
5. 状态机中的速度、下落和装备状态读取节点已接到父类属性。原 Linked Anim Graph 的 `Speed`、`isFalling` Property Access 绑定继续使用同名属性。
6. `AnimNotify_AttackEnd → SetAttackState(NoAttacking)` 仍然存在，目标引用改为父类的 `SlashCharacter`。攻击生命周期改造不包含在这一步中。
7. 打开 `BP_SlashCharacter`，确认 Mesh 的 Anim Class 仍是 `ABP_SlashCharacter`。

将来手动迁移类似蓝图时，应先记录变量读取节点的消费者，再处理同名变量和父类，最后重新连接、编译和保存。删除变量可能同时删除它的读取节点，因此不能仅修改 Parent Class 就认为迁移完成。

## 5. 验证

本次已通过 `SlashEditor Win64 Development` 构建、三个相关蓝图的重新加载和编译，以及下面两项自动化测试（2 成功，0 警告，0 失败）。测试报告位于不纳入 Git 的 `Saved/AnimationDataMigration/Automation/index.json`。

自动化测试位于 `Source/Slash/Private/Tests/SlashAnimInstanceTests.cpp`，在 Session Frontend 的 Automation 页搜索 `Slash.Animation`：

- `Slash.Animation.CharacterData`：验证原生角色拥有者、水平速度、下落、装备及攻击状态更新、解除初始化和无拥有者预览。
- `Slash.Animation.BlueprintIntegration`：加载现有角色蓝图，验证原生动画父类、动画图求值及保留的攻击结束通知。

最后在 `SlashOpenWorld` 中进行人工 PIE 检查：站立和跑动切换、起跳和落地、拾取武器后的姿势、攻击后的恢复，以及斜坡和台阶上的脚部 IK。观察 Output Log，确认没有本次迁移引起的 `Accessed None` 或动画蓝图编译错误。无渲染的自动化测试不验证最终视觉效果。
