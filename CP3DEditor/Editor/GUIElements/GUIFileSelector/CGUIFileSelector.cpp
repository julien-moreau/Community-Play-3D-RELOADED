#include "Stdafx.h"

#include "CGUIFileSelector.h"

namespace cp3d {
namespace ui {

const s32 FOD_WIDTH = 640;
const s32 FOD_HEIGHT = 400;
const s32 yoffset = 10;

s32 CGUIFileSelector::numFileSelectors = 0;

//! constructor
CGUIFileSelector::CGUIFileSelector(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id, E_FILESELECTOR_TYPE type)
: ICP3DFileSelector(environment, parent, id,
					 core::rect<s32>((parent->getAbsolutePosition().getWidth()-FOD_WIDTH)/2,
					 (parent->getAbsolutePosition().getHeight()-FOD_HEIGHT)/2,
					 (parent->getAbsolutePosition().getWidth()-FOD_WIDTH)/2+FOD_WIDTH,
					 (parent->getAbsolutePosition().getHeight()-FOD_HEIGHT)/2+FOD_HEIGHT)),
					 Dragging(false), FileNameText(0), FileList(0), DialogType(type)
{
#ifdef _DEBUG
	IGUIElement::setDebugName("CGUIFileSelector");
#endif

	Text = title;
	IsDirectoryChoosable = false;
	device=NULL;
	PlacesBox=NULL;
	default_project_dir=L"";
    
    drawWindow = true;


	IGUISkin* skin = Environment->getSkin();
	IGUISpriteBank* sprites = 0;
	video::SColor color(255,255,255,255);
	if (skin)
	{
		sprites = skin->getSpriteBank();
		color = skin->getColor(EGDC_WINDOW_SYMBOL);
	}

	s32 buttonw = Environment->getSkin()->getSize(EGDS_WINDOW_BUTTON_WIDTH);

	// Position the close button Windows or Linux style
#ifdef WIN32
	s32 posx = RelativeRect.getWidth() - buttonw - 4;
#else
	s32 posx = 4;
#endif
	CloseButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", L"Close");
	CloseButton->setSubElement(true);
	if (sprites) {
		CloseButton->setSpriteBank(sprites);
		CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), color);
		CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), color);
	}
#ifdef WIN32
	CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
#else
	CloseButton->setAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
#endif
	CloseButton->grab();

	// Ok button
	OKButton = Environment->addButton(
		core::rect<s32>(RelativeRect.getWidth()-160, RelativeRect.getHeight()-30, RelativeRect.getWidth()-90, RelativeRect.getHeight()-10),
		this, -1, (DialogType==EFST_OPEN_DIALOG?L"Open":L"Save"));
	OKButton->setSubElement(true);
	OKButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
	OKButton->grab();

	CancelButton = Environment->addButton(
		core::rect<s32>(RelativeRect.getWidth()-80, RelativeRect.getHeight()-30, RelativeRect.getWidth()-10, RelativeRect.getHeight()-10),
		this, -1, skin ? skin->getDefaultText(EGDT_MSG_BOX_CANCEL) : L"Cancel");

	CancelButton->setSubElement(true);
	CancelButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
	CancelButton->grab();

	//Description
	irr::gui::IGUIStaticText * but1 = Environment->addStaticText(L"Favorites",irr::core::rect<s32>(20,35+yoffset,170,48+yoffset),false,false,this,-1);
	irr::gui::IGUIStaticText * but2 = Environment->addStaticText(L"Files",irr::core::rect<s32>(190,35+yoffset,300,48+yoffset),false,false, this,-1);

	FileBox = Environment->addListBox(core::rect<s32>(180, 50+yoffset, RelativeRect.getWidth()-10, RelativeRect.getHeight()-60), this, -1, true);
	FileBox->setSubElement(true);
	FileBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
	FileBox->grab();

	FileNameText = Environment->addEditBox(0, core::rect<s32>(10, RelativeRect.getHeight()-30, RelativeRect.getWidth()-180, RelativeRect.getHeight()-10), true, this, -1);
	FileNameText->setSubElement(true);
	FileNameText->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
	FileNameText->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	FileNameText->grab();

	//FileNameText = Environment->addEditBox(0, core::rect<s32>(10, 30, RelativeRect.getWidth()-90, 50), true, this, -1);
	PathNameText = Environment->addEditBox(0, core::rect<s32>(10, 10+yoffset, RelativeRect.getWidth()-10, 30+yoffset), true, this, -1);
	PathNameText->setSubElement(true);
	PathNameText->setAlignment(EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	PathNameText->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	PathNameText->grab();

	FilterComboBox = Environment->addComboBox(core::rect<s32>(RelativeRect.getWidth()-160, RelativeRect.getHeight()-55, RelativeRect.getWidth()-10, RelativeRect.getHeight()-35), this, -1);
	FilterComboBox->setSubElement(true);
	FilterComboBox->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
	FilterComboBox->grab();
	FilterComboBox->addItem(L"All Files");

	// Contain the names of the favorites folders
	PlacesBox = Environment->addListBox(core::rect<s32>(10, 50+yoffset, 170, RelativeRect.getHeight()-60), this, -1, true);
	PlacesBox->setSubElement(true);
	PlacesBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
	PlacesBox->grab();

// This is for Windows machines, so they can easily select their drive from a list
#ifdef _IRR_WINDOWS_
	DriveBox = Environment->addComboBox(core::rect<s32>(10, RelativeRect.getHeight()-55, 170, RelativeRect.getHeight()-35), this, -1);
	DriveBox->setSubElement(true);
	DriveBox->setAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);
	DriveBox->grab();
	populatePCDrives();
#endif

	core::stringc str = "FileSelectorIcons";
	str += numFileSelectors++;
	SpriteBank = Environment->addEmptySpriteBank(str.c_str());
	if (SpriteBank) {
		SpriteBank->grab();
		FileBox->setSpriteBank(SpriteBank);
		PlacesBox->setSpriteBank(SpriteBank);
	}
	DirectoryIconIdx = -1;
	FileIconIdx = -1;

	FileSystem = Environment->getFileSystem();

	if (FileSystem) {
		FileSystem->grab();
		prev_working_dir = FileSystem->getWorkingDirectory();
		//printf("working directory saved: %s\n", prev_working_dir.c_str());
	}

	fillListBox();
	// Will set the focus on the text box on initialisation.
	Environment->setFocus(FileNameText);



#ifndef _IRR_WINDOWS_

#endif

}



//! destructor
CGUIFileSelector::~CGUIFileSelector()
{
	if (CloseButton)
		CloseButton->drop();

	if (OKButton)
		OKButton->drop();

	if (CancelButton)
		CancelButton->drop();

	if (FileBox)
		FileBox->drop();

	if (FileNameText)
		FileNameText->drop();

	if (PathNameText)
		PathNameText->drop();

	if (FileSystem)
		FileSystem->drop();

	if (FileList)
		FileList->drop();

	if (FilterComboBox)
		FilterComboBox->drop();

	if (PlacesBox)
		PlacesBox->drop();

	if (DriveBox)
		DriveBox->drop();

	if (SpriteBank)
		SpriteBank->drop();

}


//! returns the filename of the selected file. Returns NULL, if no file was selected.
const wchar_t* CGUIFileSelector::getFileName() const
{
	return (fullpathname.c_str());
}



//! called if an event happened.
bool CGUIFileSelector::OnEvent(const SEvent& event)
{
	switch(event.EventType)
	{
	case EET_KEY_INPUT_EVENT:
		switch (event.KeyInput.Key)
		{
		case KEY_RETURN:
			if (FileSystem)
			{
				
				FileSystem->changeWorkingDirectoryTo(core::stringc(FileNameText->getText()).c_str());
				fillListBox();
				PathNameText->setText(core::stringw(FileSystem->getWorkingDirectory()).c_str());
				printf ("Enter key being pressed\n");
				if (IsDirectoryChoosable || matchesFileFilter(FileNameText->getText()))
				{
					// Will save the file if the user press enter key.
					core::stringw strw = FileSystem->getWorkingDirectory();
					if (strw[strw.size()-1] != '/')
						strw += "/";
					fullpathname = strw+FileNameText->getText();
					sendSelectedEvent();
					remove();
					return true;
				} else
				{
					// If the user did not enter the extension, will add it automaticaly
					u32 i = FilterComboBox->getSelected();
					core::stringw strw = FileSystem->getWorkingDirectory();
					if (strw[strw.size()-1] != '/')
						strw += "/";
					fullpathname = strw+FileNameText->getText();
					fullpathname+=".";
					fullpathname+=FileFilters[i].FileExtension;
					sendSelectedEvent();
					remove();
					return true;
				}
			}
			return true;
		}
		break;
	case EET_GUI_EVENT:
		switch(event.GUIEvent.EventType)
		{
		case EGET_COMBO_BOX_CHANGED:
			if (event.GUIEvent.Caller == FilterComboBox)
			{
				fillListBox();
			} else {  // change drive
				if (FileSystem) {
					FileSystem->changeWorkingDirectoryTo(core::stringc(DriveBox->getText()).c_str());
					fillListBox();
				}
			}
			break;

		case EGET_ELEMENT_FOCUS_LOST:
			Dragging = false;
			break;

		case EGET_BUTTON_CLICKED:
			if (event.GUIEvent.Caller == CloseButton ||
				event.GUIEvent.Caller == CancelButton)
			{
				if (FileSystem)
				{
					//FileSystem->changeWorkingDirectoryTo(prev_working_dir.c_str());
					//printf("working directory reset to: %s\n", prev_working_dir.c_str());
				}
				sendCancelEvent();
				remove();
				return true;
			}
			else
				// If the user enter the complete extension of the filename (mostly for loading)
				if (event.GUIEvent.Caller == OKButton && (IsDirectoryChoosable || matchesFileFilter(FileNameText->getText()))) {
					if (FileSystem)
					{
						//FileSystem->changeWorkingDirectoryTo(prev_working_dir.c_str());
						//printf("working directory reset to: %s\n", prev_working_dir.c_str());
					}
					if (DialogType==EFST_SAVE_DIALOG)
					{
						core::stringw strw = FileSystem->getWorkingDirectory();
						if (strw[strw.size()-1] != '/')
							strw += "/";
						fullpathname = strw+FileNameText->getText();
					}
					sendSelectedEvent();
					remove();
					return true;
				} else
				// In case the user did not enter the extension (assumed it would not enter it)
				if (DialogType==EFST_SAVE_DIALOG)
				{
					u32 i = FilterComboBox->getSelected();
					core::stringw strw = FileSystem->getWorkingDirectory();
					if (strw[strw.size()-1] != '/')
						strw += "/";
					fullpathname = strw+FileNameText->getText();
					fullpathname+=".";
					fullpathname+=FileFilters[i].FileExtension;
					sendSelectedEvent();
					remove();
					return true;
				}
				break;

		case EGET_LISTBOX_CHANGED:
			{
				if (event.GUIEvent.Caller == FileBox)
				{
					s32 selected = FileBox->getSelected();
					if (FileList && FileSystem)
					{
						this->fullpathname=FileSystem->getWorkingDirectory();
						core::stringw strw;
						strw = FileSystem->getWorkingDirectory();
						if (strw[strw.size()-1] != '/')
							strw += "/";
						fullpathname = strw+FileBox->getListItem(selected);
						FileNameText->setText(FileBox->getListItem(selected));
						PathNameText->setText(strw.c_str());
					}
				}
				if (event.GUIEvent.Caller == PlacesBox)
				{
					s32 selected = PlacesBox->getSelected();
					FileSystem->changeWorkingDirectoryTo(core::stringc(placespaths[selected]).c_str());
					fillListBox();
				}
			}
			break;

		case EGET_LISTBOX_SELECTED_AGAIN:
			{
				s32 selected = FileBox->getSelected();
				if (event.GUIEvent.Caller == FileBox)
				{
					s32 selected = FileBox->getSelected();
					if (FileList && FileSystem)
					{
						if (FileList->isDirectory(selected))
						{
							FileSystem->changeWorkingDirectoryTo(FileList->getFileName(selected));
							PathNameText->setText(core::stringw(FileSystem->getWorkingDirectory()).c_str());
							fillListBox();
							FileNameText->setText(L"");
						}
						else
						{
							this->fullpathname=FileSystem->getWorkingDirectory();
							core::stringw strw;
							strw = FileSystem->getWorkingDirectory();
							if (strw[strw.size()-1] != '/')
								strw += "/";
							fullpathname = strw+FileBox->getListItem(selected);
							FileNameText->setText(FileBox->getListItem(selected));
							PathNameText->setText(strw.c_str());
							return true;
						}
					}
				}
				if (event.GUIEvent.Caller == PlacesBox)
				{
					s32 selected = PlacesBox->getSelected();
					FileSystem->changeWorkingDirectoryTo(core::stringc(placespaths[selected]).c_str());
					fillListBox();
				}
			}
			break;
		}
		break;

	case EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			DragStart.X = event.MouseInput.X;
			DragStart.Y = event.MouseInput.Y;
			Dragging = true;
			Environment->setFocus(this);
			return true;
		case EMIE_LMOUSE_LEFT_UP:
			Dragging = false;
			Environment->removeFocus(this);
			return true;
		case EMIE_MOUSE_MOVED:
			if (Dragging && !stretchbottom && !stretchtop  && !stretchright && !stretchleft)
			{
				// gui window should not be dragged outside its parent
				if (Parent)
					if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
						event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
						event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
						event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1)

						return true;

				move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
				DragStart.X = event.MouseInput.X;
				DragStart.Y = event.MouseInput.Y;
				return true;
			}
			break;

		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}


//! draws the element and its children
void CGUIFileSelector::draw()
{
	if (!IsVisible)
		return;

	core::vector2d<s32> mousepos = core::vector2d<s32>(0,0);
	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();
	if (device)
		mousepos=device->getCursorControl()->getPosition();

	core::rect<s32> rect = AbsoluteRect;
    
    if (drawWindow) {
        rect = skin->draw3DWindowBackground(this, true, skin->getColor(EGDC_ACTIVE_BORDER), rect, &AbsoluteClippingRect);
    } else {
        CloseButton->setVisible(false);
        Dragging = false;
    }

	if (Text.size())
	{
#ifdef WIN32
		rect.UpperLeftCorner.X += 2;
#else
		rect.UpperLeftCorner.X += CloseButton->getAbsoluteClippingRect().getWidth()+10;
#endif
		rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

		IGUIFont* font = skin->getFont(EGDF_WINDOW);
		if (font)
			font->draw(Text.c_str(), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true,
			&AbsoluteClippingRect);
	}

	// Get the current mousecursor position;

	if (!Dragging)
	{
		//Hinting and detection for stretch on the right side
		if ((mousepos.X>=AbsoluteRect.LowerRightCorner.X-10) && (mousepos.X<=AbsoluteRect.LowerRightCorner.X-2))
		{
			driver->draw2DLine(core::vector2d<s32>(AbsoluteRect.LowerRightCorner.X-1,AbsoluteRect.UpperLeftCorner.Y+1),core::vector2d<s32>(AbsoluteRect.LowerRightCorner.X-1,AbsoluteRect.LowerRightCorner.Y-1),video::SColor(255,255,255,0));
			this->stretchright=true;
		} else
			this->stretchright=false;

		//Hinting and detection for stretch on the left side
		if ((mousepos.X>=AbsoluteRect.UpperLeftCorner.X+2) && (mousepos.X<=AbsoluteRect.UpperLeftCorner.X+10))
		{
			driver->draw2DLine(core::vector2d<s32>(AbsoluteRect.UpperLeftCorner.X+1,AbsoluteRect.LowerRightCorner.Y-1),core::vector2d<s32>(AbsoluteRect.UpperLeftCorner.X+1,AbsoluteRect.UpperLeftCorner.Y-1),video::SColor(255,255,255,0));
			this->stretchleft=true;
		} else
			this->stretchleft=false;

		//Hinting and detection for stretch on the bottom
		if ((mousepos.Y>=AbsoluteRect.LowerRightCorner.Y-10) && (mousepos.Y<=AbsoluteRect.LowerRightCorner.Y-2))
		{
			driver->draw2DLine(core::vector2d<s32>(AbsoluteRect.UpperLeftCorner.X+1,AbsoluteRect.LowerRightCorner.Y-1),core::vector2d<s32>(AbsoluteRect.LowerRightCorner.X-1,AbsoluteRect.LowerRightCorner.Y-1),video::SColor(255,255,255,0));
			this->stretchbottom=true;
		}
		else
			this->stretchbottom=false;

		//Hinting and detection for stretch on the top
		if ((mousepos.Y<=AbsoluteRect.UpperLeftCorner.Y+5) && (mousepos.Y>=AbsoluteRect.UpperLeftCorner.Y+1))
		{
			driver->draw2DLine(core::vector2d<s32>(AbsoluteRect.UpperLeftCorner.X+1,AbsoluteRect.UpperLeftCorner.Y+1),core::vector2d<s32>(AbsoluteRect.LowerRightCorner.X-1,AbsoluteRect.UpperLeftCorner.Y+1),video::SColor(255,255,255,0));
			this->stretchtop=true;
		}
		else
			this->stretchtop=false;
	}
	else
	{
		if (!stretchtop && !stretchbottom && !stretchright && !stretchleft)
		{
			// gui window should not be dragged outside its parent
			if (Parent)
			{
				if (mousepos.X < Parent->getAbsolutePosition().UpperLeftCorner.X +30 ||
					mousepos.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +30 ||
					mousepos.X > Parent->getAbsolutePosition().LowerRightCorner.X -30 ||
					mousepos.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -30)
				{
					IGUIElement::draw();
					return;
				}
			}


			move(core::position2d<s32>(mousepos.X - DragStart.X, mousepos.Y - DragStart.Y));
			updateAbsolutePosition();
			DragStart.X = mousepos.X;
			DragStart.Y = mousepos.Y;
			IGUIElement::draw();
			return;
		}

		if (stretchright)
		{
			if (mousepos.X<10 || mousepos.X>this->getParent()->getAbsoluteClippingRect().getWidth()-10)
			{
				IGUIElement::draw();
				return;
			}

			if ((mousepos.X-AbsoluteRect.UpperLeftCorner.X)>200)
			{
				AbsoluteRect.LowerRightCorner.X=mousepos.X+5;
			}
			this->AbsoluteClippingRect=this->AbsoluteRect;
			this->DesiredRect=AbsoluteRect;
			this->updateAbsolutePosition();
		}
		if (stretchleft)
		{
			if (mousepos.X<10 || mousepos.X>this->getParent()->getAbsoluteClippingRect().getWidth()-10)
			{
				IGUIElement::draw();
				return;
			}

			if ((AbsoluteRect.LowerRightCorner.X-mousepos.X)>200)
			{
				AbsoluteRect.UpperLeftCorner.X=mousepos.X-5;
			}
			this->AbsoluteClippingRect=this->AbsoluteRect;
			this->DesiredRect=AbsoluteRect;
			this->updateAbsolutePosition();
		}
		if (stretchbottom)
		{
			if (mousepos.Y<10 || mousepos.Y>this->getParent()->getAbsoluteClippingRect().getHeight()-10)
			{
				IGUIElement::draw();
				return;
			}

			if ((mousepos.Y-AbsoluteRect.UpperLeftCorner.Y)>200)
			{
				AbsoluteRect.LowerRightCorner.Y=mousepos.Y+5;
			}
			this->AbsoluteClippingRect=this->AbsoluteRect;
			this->DesiredRect=AbsoluteRect;
			this->updateAbsolutePosition();
		}
		if (stretchtop)
		{
			if (mousepos.Y<10 || mousepos.Y>this->getParent()->getAbsoluteClippingRect().getHeight()-10)
			{
				IGUIElement::draw();
				return;
			}

			if ((AbsoluteRect.LowerRightCorner.Y-mousepos.Y)>200)
			{
				AbsoluteRect.UpperLeftCorner.Y=mousepos.Y-5;
			}
			this->AbsoluteClippingRect=this->AbsoluteRect;
			this->DesiredRect=AbsoluteRect;
			this->updateAbsolutePosition();
		}
	}

	IGUIElement::draw();
}

//bool CGUIFileSelector::matchesFileFilter(core::stringw s) {
//  if (FilterComboBox->getSelected() >= FileFilters.size()) return true; // 'All Files' selectable
//  else {
//    s32 pos = s.findLast('.'); // Find the last '.' so we can check the file extension
//    return FileFilters[FilterComboBox->getSelected()].FileExtension.equals_ignore_case(core::stringw(&s.c_str()[pos+1]));
//  }
//}

bool CGUIFileSelector::matchesFileFilter(core::stringw s)
{
	if (FileFilters.size() > 1)
	{
		s32 selected = FilterComboBox->getSelected();
		if (selected == 0)
		{
			for (u32 i = 0; i < FileFilters.size(); i++)
			{
				s32 pos = s.findLast('.'); // Find the last '.' so we can check the file extension
				if (FileFilters[i].FileExtension.equals_ignore_case(core::stringw(&s.c_str()[pos+1])))
					return true;
			}
			return false;
		}
		selected--;
		if (selected >= (s32)FileFilters.size()) return true; // 'All Files' selectable
		else {
			s32 pos = s.findLast('.'); // Find the last '.' so we can check the file extension
			return FileFilters[selected].FileExtension.equals_ignore_case(core::stringw(&s.c_str()[pos+1]));
		}
	}
	if (FilterComboBox->getSelected() >= (s32)FileFilters.size()) return true; // 'All Files' selectable
	else {
		s32 pos = s.findLast('.'); // Find the last '.' so we can check the file extension
		return FileFilters[FilterComboBox->getSelected()].FileExtension.equals_ignore_case(core::stringw(&s.c_str()[pos+1]));
	}
}

bool CGUIFileSelector::matchesFileFilter(core::stringw s, core::stringw f)
{
	s32 pos = s.findLast('.'); // Find the last '.' so we can check the file extension
	return f.equals_ignore_case(core::stringw(&s.c_str()[pos+1]));
}

//! fills the listbox with files.
void CGUIFileSelector::fillListBox()
{
	IGUISkin *skin = Environment->getSkin();

	if (!FileSystem || !FileBox || !skin)
		return;

	if (FileList)
		FileList->drop();

	FileBox->clear();

	FileList = FileSystem->createFileList();
	core::stringw s;

	for (u32 i=0; i<FileList->getFileCount(); ++i)
	{
		s = FileList->getFileName(i);
		core::stringw test2=translateDOS(FileList->getFileName(i));
		s = test2;
		// We just want a list of directories and those matching the file filter
		if (FileList->isDirectory(i))
		{
			if (DirectoryIconIdx != -1)
				FileBox->addItem(s.c_str(), DirectoryIconIdx);
			else
				FileBox->addItem(s.c_str());
		}
		else if (matchesFileFilter(s))
		{
			if (FilterComboBox->getSelected() >= (s32)FileFilters.size())
			{
				if (FileIconIdx != -1)
				{
					s32 iconIdx = FileIconIdx;
					for (u32 i = 0 ; i < FileFilters.size() ; i++)
					{
						if (matchesFileFilter(s, FileFilters[i].FileExtension))
							iconIdx = FileFilters[i].FileIconIdx;
					}
					FileBox->addItem(s.c_str(), iconIdx);
				}
				else
				{
					FileBox->addItem(s.c_str());
				}
			} else FileBox->addItem(s.c_str(), FileFilters[FilterComboBox->getSelected()].FileIconIdx);
		}
	}

	if (PathNameText)
	{
		s = FileSystem->getWorkingDirectory();
		PathNameText->setText(s.c_str());
	}
}


//! sends the event that the file has been selected.
void CGUIFileSelector::sendSelectedEvent()
{
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.EventType = EGET_FILE_SELECTED;
	if (Parent)
		Parent->OnEvent(event);
	this->setVisible(false);
}

//! sends the event that the file choose process has been canceld
void CGUIFileSelector::sendCancelEvent()
{
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.EventType = EGET_FILE_CHOOSE_DIALOG_CANCELLED;
	Parent->OnEvent(event);
	this->setVisible(false);
}


void CGUIFileSelector::addFileFilter(wchar_t* name, wchar_t* ext, video::ITexture* texture)
{
	SFileFilter filter(name, ext, texture);

	filter.FileIconIdx = addIcon(texture);

	FileFilters.push_back(filter);

	FilterComboBox->clear();
	core::stringw strw;

	if (FileFilters.size() > 1) {
		strw = "Supported ";
		for (u32 i = 0 ; i < FileFilters.size() ; i++) {
			strw += ".";
			strw += FileFilters[i].FileExtension;
			strw += " ";
		}
		FilterComboBox->addItem(strw.c_str());
	}

	for (u32 i = 0 ; i < FileFilters.size() ; i++) {
		strw = FileFilters[i].FilterName;
		strw += " (*.";
		strw += FileFilters[i].FileExtension;
		strw += ")";
		FilterComboBox->addItem(strw.c_str());
	}
	FilterComboBox->addItem(L"All Files");

	fillListBox();
}

u32 CGUIFileSelector::addIcon(video::ITexture* texture)
{
	if (!SpriteBank || !texture) return 0;

	// load and add the texture to the bank
	SpriteBank->addTexture(texture);
	u32 textureIndex = SpriteBank->getTextureCount() - 1;
	// now lets get the sprite bank's rectangles and add some for our animation
	core::array<core::rect<s32> >& rectangles = SpriteBank->getPositions();
	u32 firstRect = rectangles.size();
	// remember that rectangles are not in pixels, they enclose pixels!
	// to draw a rectangle around the pixel at 0,0, it would rect<s32>(0,0, 1,1)
	rectangles.push_back(core::rect<s32>(0,0, 16,16));


	// now we make a sprite..
	SGUISprite sprite;
	sprite.frameTime = 30;
	// add some frames of animation.
	SGUISpriteFrame frame;
	frame.rectNumber = firstRect;
	frame.textureNumber = textureIndex;

	// add this frame
	sprite.Frames.push_back(frame);
	// add the sprite
	u32 spriteIndex = SpriteBank->getSprites().size();
	SpriteBank->getSprites().push_back(sprite);

	return textureIndex;
}

// This will add items in the favorites folders lists (or places)
void CGUIFileSelector::addPlacePaths(wchar_t* name, core::stringw path, video::ITexture* texture)
{

	if (texture)
		PlacesBox->addItem(name, addIcon(texture));
	else
		PlacesBox->addItem(name);

	core::stringc newpath = "";
	if (FileSystem)
	{
		FileSystem->changeWorkingDirectoryTo(core::stringc(path).c_str());

		newpath = core::stringc(FileSystem->getWorkingDirectory()).c_str();
		fillListBox();
		FileSystem->changeWorkingDirectoryTo(core::stringc(prev_working_dir).c_str());
	}

	placespaths.push_back(newpath);
}

//! Returns the directory of the selected file. Returns NULL, if no directory was selected.
const io::path& CGUIFileSelector::getDirectoryName()
{
	FileSystem->flattenFilename ( FileDirectory );
	return FileDirectory;
}


void CGUIFileSelector::populatePCDrives()
{
#ifdef WIN32
	int dr_type=99;
	wchar_t dr_avail[1024];
	wchar_t *temp=dr_avail;

	GetLogicalDriveStrings(32, dr_avail);
	while(*temp!=NULL) { // Split the buffer by null
		dr_type=GetDriveType((LPWSTR)temp);
		switch(dr_type) {
			case 0: // Unknown
				printf("%ls : Unknown Drive type\n",temp);
				break;

			case 1: // Invalid
				printf("%ls : Drive is invalid\n",temp);
				break;

			case 2: // Removable Drive
				printf("%ls : Removable Drive\n",temp);
				DriveBox->addItem(temp);
				break;

			case 3: // Fixed
				printf("%ls : Hard Disk (Fixed)\n",temp);
				DriveBox->addItem(temp);
				break;

			case 4: // Remote
				printf("%ls : Remote (Network) Drive\n",temp);
				DriveBox->addItem(temp);
				break;

			case 5: // CDROM
				printf("%ls : CD-Rom/DVD-Rom\n",temp);
				DriveBox->addItem(temp);
				break;

			case 6: // RamDrive
				printf("%ls : Ram Drive\n",temp);
				DriveBox->addItem(temp);
				break;

		}
		temp += lstrlen((LPCWSTR)temp) +1; // incriment the buffer
	}
#endif
}

void CGUIFileSelector::populateWindowsFAV()
{
#ifdef WIN32
	// Get the desktop shortcut (places)
	// String buffer for holding the path.
	TCHAR strPath[ MAX_PATH ];

	irr::video::IVideoDriver * driver = Environment->getVideoDriver();

	// Get the special folder path. (Desktop)
	SHGetSpecialFolderPath(	0, strPath, CSIDL_DESKTOPDIRECTORY, FALSE );
	addPlacePaths(L"Desktop",(wchar_t *)strPath,driver->getTexture("../media/art/places_desktop.png"));

	// Get the special folder path. (My documents)
	SHGetSpecialFolderPath(	0, strPath, CSIDL_PERSONAL, FALSE );
	addPlacePaths(L"My documents",(wchar_t *)strPath,driver->getTexture("../media/art/places_documents.png"));

	// Get the special folder path. (My pictures)
	SHGetSpecialFolderPath( 0, strPath, CSIDL_MYPICTURES, FALSE );
	addPlacePaths(L"My pictures",(wchar_t *)strPath,driver->getTexture("../media/art/places_pictures.png"));

	// Get the special folder path. (My music)
	SHGetSpecialFolderPath(	0, strPath, CSIDL_MYMUSIC, FALSE );
	addPlacePaths(L"My music",(wchar_t *)strPath,driver->getTexture("../media/art/places_folder.png"));

	// Get the special folder path. (My video)
	SHGetSpecialFolderPath( 0, strPath, CSIDL_MYVIDEO, FALSE );
	addPlacePaths(L"My videos",(wchar_t *)strPath,driver->getTexture("../media/art/places_folder.png"));

	// Get the special folder path. (Public documents)
	SHGetSpecialFolderPath(	0, strPath, CSIDL_COMMON_DOCUMENTS, FALSE );
	addPlacePaths(L"Public documents",(wchar_t *)strPath,driver->getTexture("../media/art/places_folder.png"));

	// Get the special folder path. (Public pictures)
	SHGetSpecialFolderPath(	0, strPath, CSIDL_COMMON_PICTURES, FALSE );
	addPlacePaths(L"Public pictures",(wchar_t *)strPath,driver->getTexture("../media/art/places_folder.png"));

	// Get the special folder path. (Public music)
	SHGetSpecialFolderPath(	0, strPath, CSIDL_COMMON_MUSIC, FALSE );
	addPlacePaths(L"Public music",(wchar_t *)strPath,driver->getTexture("../media/art/places_folder.png"));

	// Get the special folder path. (Public videos)
	SHGetSpecialFolderPath( 0, strPath, CSIDL_COMMON_VIDEO, FALSE );
	addPlacePaths(L"Public videos",(wchar_t *)strPath,driver->getTexture("../media/art/places_folder.png"));
#endif
}

void CGUIFileSelector::populateLinuxFAV()
{


	// Some commoms places on Linux as seen on https://help.ubuntu.com/community/LinuxFilesystemTreeOverview

	irr::video::IVideoDriver * driver = Environment->getVideoDriver();

	// Get current user home folder(places)
	// Can't get the other subfolders as they are translated for each language.
	//addPlacePaths(L"User Home folder",L"~/",driver->getTexture("../media/art/places_folder.png"));

	addPlacePaths(L"Home folder",L"/home",driver->getTexture("../media/art/places_folder.png"));

	addPlacePaths(L"Media folder",L"/media",driver->getTexture("../media/art/places_folder.png"));

	addPlacePaths(L"Usr folder",L"/usr",driver->getTexture("../media/art/places_folder.png"));

	addPlacePaths(L"Dev folder",L"/dev",driver->getTexture("../media/art/places_folder.png"));

	addPlacePaths(L"Mnt folder",L"/mnt",driver->getTexture("../media/art/places_folder.png"));


}



void CGUIFileSelector::setStartingPath(core::stringw path)
{
	if (FileSystem)
	{
		FileSystem->changeWorkingDirectoryTo(core::stringc(path).c_str());
		default_project_dir = FileSystem->getWorkingDirectory();
		fillListBox();
	}
}

// !translateDOS, will try to convert accents from the received string and send them back, as they seem corrupted
// from the way it was received. (Testing to be done Linux Side)
core::stringw CGUIFileSelector::translateDOS(core::stringw input)
{
	core::stringw result=L"";

	for (u32 a=0; a <= input.size(); a++)
	{
		core::stringc test=input.subString(a,1);

		int code = int(test[0]);

		// If the "ascii" code is "normal then append the letter only
		if (code>0)
			result.append(test);
		if (code<0)
			printf("============================\nThe code is: %d\n==============================\n",code);
#ifdef WIN32
		// if the result give < 0 then it look like an accented letter, then convert
		
		if (code==-56)
			result.append(L"�");

		if (code==-55)
			result.append(L"�");

		if (code==-53)
			result.append(L"�");

		if (code==-24)
			result.append(L"�");

		if (code==-23)
			result.append(L"�");

		if (code==-21)
			result.append(L"�");

		if (code==-64)
			result.append(L"�");

		if (code==-32)
			result.append(L"�");

		if (code==-57)
			result.append(L"�");

		if (code==-25)
			result.append(L"�");

		if (code==-42)
			result.append(L"�");

		if (code==-10)
			result.append(L"�");

		if (code==-39)
			result.append(L"�");

		if (code==-7)
			result.append(L"�");
#else
		// if the result give < 0 then it look like an accented letter, then convert
		// Have to find a way on Linux for converting the accented characters to a irrlicht displayable character
		// Not implemented on Linux.
		
#endif


	}
	return result;
}

} /// End namespace ui
} /// End namespace cp3d
