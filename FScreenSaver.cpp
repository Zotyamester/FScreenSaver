#include "FScreenSaver.h"

#include <stdlib.h>

#include <Alignment.h>
#include <LayoutBuilder.h>
#include <String.h>
#include <StringView.h>
#include <Window.h>

extern "C" BScreenSaver*
instantiate_screen_saver(BMessage* msg, image_id id)
{
	return new FScreenSaver(msg, id);
}

FScreenSaver::FScreenSaver(BMessage* archive, image_id id)
	:
	BScreenSaver(archive, id)
{
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
	
	BLayoutBuilder::Group<>(view, B_VERTICAL, B_USE_ITEM_SPACING)
		.SetInsets(B_USE_WINDOW_INSETS)
		.SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP))
		.AddGroup(B_HORIZONTAL)
			.Add(v1)
			.Add(v2)
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
	color.set_to(0, 0, 0);
}

void
FScreenSaver::Draw(BView* view, int32 frame)
{
	if (fNeedsRestart)
		_Restart(view);
	view->SetHighColor(color);
	view->FillRect(view->Bounds());
	switch (mode) {
		case 0:
			if (++color.red == 255)
				mode = 1;
			break;
		case 1:
			if (--color.red == 0)
				mode = 2;
			break;
		case 2:
			if (++color.green == 255)
				mode = 3;
			break;
		case 3:
			if (--color.green == 0)
				mode = 4;
			break;
		case 4:
			if (++color.blue == 255)
				mode = 5;
			break;
		case 5:
			if (--color.blue == 0)
				mode = 0;
			break;
	}
}

void
FScreenSaver::MessageReceived(BMessage* msg)
{
	BHandler::MessageReceived(msg);
}

status_t FScreenSaver::SaveState(BMessage* into) const
{
	return B_OK;
}
