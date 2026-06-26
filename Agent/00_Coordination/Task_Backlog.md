# 任务池

*最后更新：2026-06-26*

## Phase 0 - 协作与项目基线

**目标：** 在改动玩法代码之前，先建立协作规则、项目上下文和当前事实基线。

状态：进行中

任务：
- [x] 从代码库扫描生成 `.agents/ue-project-context.md` V1。
- [x] 创建 Agent 协作文档目录和说明。
- [x] 创建协作约定。
- [x] 创建 Git 工作流建议。
- [x] 创建 Skill/MCP 目录。
- [x] 创建当前 Aura 项目状态摘要。
- [x] 审查当前 dirty 工作区，判断哪些内容应该先提交。
- [ ] 评审 `Agent/UE_GAS_SkillGraph_Design_Document.md`，只把它作为参考，不直接照搬。

## Phase 1 - 技能机制调研与分类

**目标：** 在架构设计前，先整理市面常见技能机制，建立分类表。

候选产出：
- `Agent/02_SkillSystem/Skill_Mechanic_Taxonomy.md`
- 第一批优先原型清单。
- 哪些机制会重点考验目标选择、时序、联网、动画、特效、UI。

示例主题：
- 瞬发技能、投射物技能、范围技能、持续引导、蓄力释放。
- Buff、Debuff、护盾、治疗、资源消耗、光环、开关型技能。
- 召唤物、陷阱、地雷、地面区域、延迟爆炸。
- 位移、冲刺、传送、牵引、击退。
- 连招、姿态、被动触发、反击。
- 目标选择模式和确认流程。

## Phase 2 - GAS 运行时架构方案

**目标：** 对比可行架构，并选定 V1 运行时模型。

候选产出：
- `Agent/02_SkillSystem/GAS_Architecture_Options.md`
- `Agent/02_SkillSystem/Skill_Runtime_Model.md`
- 第一份 Claude 实施路线图。

需要对比：
- 每个技能一个 GA 类。
- 少量通用 GA 类 + DataAsset 配置。
- 技能图/节点解释器。
- 分阶段混合方案。

## Phase 3 - 运行时原型实现

**目标：** 实现最小但有价值的技能运行时原型。

候选产出：
- 新的运行时数据资产和执行类。
- 覆盖多个机制类型的技能原型。
- 测试或编辑器验证步骤。
- Claude 实施报告。

## Phase 4 - 资产与表现层整合

**目标：** 把动画、Niagara、音效、UI、目标预览、GameplayCue 接入运行时模型。

候选产出：
- 资产盘点。
- GameplayCue 命名和使用规范。
- 表现层接口。
- 原型技能表现示例。

## Phase 5 - 技能编辑器规划

**目标：** 在运行时模型稳定后，再设计技能编辑器。

候选产出：
- `Agent/03_SkillEditor/Editor_Requirements.md`
- `Agent/03_SkillEditor/Editor_Architecture_Options.md`
- `Agent/03_SkillEditor/Preview_Runtime_Design.md`

## Phase 6 - 技能编辑器实现

**目标：** 构建一个可以创建和修改技能配置资产的编辑器，不生成静态 C++ 技能类。

候选产出：
- Editor 模块或编辑器插件。
- Asset Type Actions / 自定义资产编辑器 / 图编辑器 UI。
- 预览工具和数据校验。
