#include "FScreenSaver.h"

#include <stdlib.h>

#include <Alignment.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <String.h>
#include <StringView.h>
#include <Window.h>

static const int32 kSlider = 'sldr';
static const int32 kDefaults = 'dflt';

extern "C" BScreenSaver*
instantiate_screen_saver(BMessage* msg, image_id id)
{
	return new FScreenSaver(msg, id);
}

FScreenSaver::FScreenSaver(BMessage* archive, image_id id)
	:
	BScreenSaver(archive, id)
{
	fDelta = 1;

	fDelta = archive->GetInt32("delta", fDelta);
}

FScreenSaver::~FScreenSaver()
{
}

void
FScreenSaver::StartConfig(BView* view)
{
	BWindow* win = view->Window();
	if (win)
		win->AddHandler(this);
	
	BStringView* v1 = new BStringView("name", "FScreenSaver");
	v1->SetFont(be_bold_font);
	BStringView* v2 = new BStringView("author", "by Zoltan Szatmary");
	v2->SetFont(be_bold_font);
	BStringView* v3 = new BStringView("delta", "Delta");
	s1 = new BSlider("slider", "", new BMessage(kSlider), 5, 64, B_HORIZONTAL);
	s1->SetValue(fDelta);
	s1->SetTarget(this);
	s1->SetLimitLabels("small", "big");
	
	BButton* b1 = new BButton("defaults", "Defaults", new BMessage(kDefaults));
	b1->SetTarget(this);
	
	BLayoutBuilder::Group<>(view, B_VERTICAL, B_USE_ITEM_SPACING)
		.SetInsets(B_USE_WINDOW_INSETS)
		.SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP))
		.AddGroup(B_HORIZONTAL)
			.Add(v1)
			.Add(v2)
			.AddGlue()
		.End()
		.Add(v3)
		.Add(s1)
		.AddGlue()
		.AddGroup(B_HORIZONTAL)
			.Add(b1)
			.AddGlue()
		.End();
}

status_t FScreenSaver::StartSaver(BView* view, bool preview)
{
	BRect rect = view->Bounds();
	fWidth = (int) rect.Width() + 1;
	fHeight = (int) rect.Height() + 1;

	view->SetDrawingMode(B_OP_ALPHA);
	view->SetLineMode(B_ROUND_CAP, B_ROUND_JOIN);
	view->SetFlags(view->Flags() | B_SUBPIXEL_PRECISE);
	
	_Restart(view);

	return B_OK;
}

void
FScreenSaver::_Restart(BView* view)
{
	color.set_to(0, 0, 0, 255);
	mode = 0;
}

void
FScreenSaver::IncColor(uint8* comp)
{
	int val = *comp + fDelta;
	if (val > 255) {
		*comp = 255;
		mode++;
	} else {
		*comp = val;
	}
}

void
FScreenSaver::DecColor(uint8* comp)
{
	int val = *comp - fDelta;
	if (val < 0) {
		*comp = 0;
		mode++;
	} else {
		*comp = val;
	}
}

void
FScreenSaver::Draw(BView* view, int32 frame)
{
	view->SetHighColor(color);
	view->FillRect(view->Bounds());
	switch (mode) {
		case 6:
			mode = 0;
		case 0:
			IncColor(&color.red);
			break;
		case 1:
			DecColor(&color.red);
			break;
		case 2:
			IncColor(&color.green);
			break;
		case 3:
			DecColor(&color.green);
			break;
		case 4:
			IncColor(&color.blue);
			break;
		case 5:
			DecColor(&color.blue);
			break;
	}
}

void
FScreenSaver::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kSlider:
			fDelta = msg->GetInt32("be:value", fDelta);
			break;
		case kDefaults:
			fDelta = 1;
			s1->SetValue(fDelta);
			break;
		default:
			BHandler::MessageReceived(msg);
			break;
	}
}

status_t FScreenSaver::SaveState(BMessage* into) const
{
	into->AddInt32("delta", fDelta);
	return B_OK;
}
