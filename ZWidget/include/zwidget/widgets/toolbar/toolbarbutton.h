
#pragma once

#include "zwidget/core/widget.h"
#include "zwidget/widgets/textlabel/textlabel.h"
#include "zwidget/widgets/imagebox/imagebox.h"

class ToolbarButton : public Widget
{
public:
	ToolbarButton(Widget* parent);
	~ToolbarButton();

	void SetIcon(std::string icon);
	void SetText(std::string text);

	void Click();

	double GetPreferredWidth() override;
	double GetPreferredHeight() override;

	std::function<void()> OnClick;

protected:
	void OnMouseMove(const Point& pos) override;
	void OnMouseLeave() override;
	bool OnMouseDown(const Point& pos, InputKey key) override;
	bool OnMouseUp(const Point& pos, InputKey key) override;
	void OnGeometryChanged() override;

private:
	ImageBox* image = nullptr;
	TextLabel* label = nullptr;
};
