#include <Handler.h>
#include <ScreenSaver.h>
#include <ObjectList.h>
#include <Slider.h>

class FScreenSaver : public BScreenSaver, public BHandler {
public:
						FScreenSaver(BMessage* archive, image_id id);
						~FScreenSaver();

	virtual void		StartConfig(BView* view);

	virtual status_t	StartSaver(BView* view, bool preview);
	virtual void		Draw(BView* view, int32 frame);

	virtual void		MessageReceived(BMessage* msg);
	virtual status_t	SaveState(BMessage* into) const;
	
	void				IncColor(uint8* comp);
	void				DecColor(uint8* comp);
private:
	void				_Restart(BView* view);

	BSlider*			s1;

	int					fWidth;
	int 				fHeight;
	int					fDelta;
	rgb_color			color;
	int					mode;
};
