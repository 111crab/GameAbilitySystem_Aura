// Copyright CRABOI


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	// WidgetController被正确设置之后，需要立刻调用WidgetControllerSet()来处理某些视觉效果或者计算逻辑。
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
