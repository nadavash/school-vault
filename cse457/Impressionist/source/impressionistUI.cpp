//
// impressionistUI.cpp
//
// The user interface part for the program.
//

#include <string>

#include <FL/fl_ask.H>

#include <algorithm>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

#include "dialog.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}

//------------------------------------------------------------------------
// This displays a modal file chooser with a native look-and-feel.
// The available dialog types are:
//   BROWSE_DIRECTORY       - Open a single directory
//   BROWSE_FILE            - Open a single file
//   BROWSE_MULTI_DIRECTORY - Open directories, allowing the user to
//                            select more than one at a time
//   BROWSE_MULTI_FILE      - Open files, allowing the user to select
//                            more than one at a time
//   BROWSE_SAVE_DIRECTORY  - Save a directory
//   BROWSE_SAVE_FILE       - Save a file
//
// The filter limits the displayed files. See cb_load_image for an example.
// title is optional, use NULL for the OS default title.
// The return value is the filepath.
//------------------------------------------------------------------------
const char* ImpressionistUI::fileDialog(Fl_Native_File_Chooser::Type dialogType, const char* filter, const char* title) {
	if(!m_nativeChooser)
		m_nativeChooser = new Fl_Native_File_Chooser(dialogType);
	else
		m_nativeChooser->type(dialogType);
	m_nativeChooser->filter(filter);
	m_nativeChooser->title(title);
	int ret = m_nativeChooser->show();
	if(ret == -1 ||	ret == 1) {
		//error or cancel respectively
		return NULL;
	}
	return m_nativeChooser->filename();
}

//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();
	/*pDoc->m_pUI->m_chooser = new Fl_File_Chooser(".", "*.{png,jpg,bmp}", 0, "Open File?" );
    pDoc->m_pUI->m_chooser->show();
    while(pDoc->m_pUI->m_chooser->shown())
        { Fl::wait(); }
	const char *filename = pDoc->m_pUI->m_chooser->value();
	while (pDoc->m_pUI->m_chooser->visible()) {
		pDoc->m_pUI->m_chooser->preview(0);
		Fl::wait();
	}
	if (filename) {
		pDoc->loadImage(filename);
	}*/
	const char* filename = pDoc->m_pUI->fileDialog( Fl_Native_File_Chooser::BROWSE_FILE,
													"Image File (*.png; *.jpg, *.bmp)\t*.{png,jpg,bmp}");
	if(filename)
		pDoc->loadImage(filename);
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();
	/*pDoc->m_pUI->m_chooser = new Fl_File_Chooser(".", "*.png\t*.jpg", Fl_File_Chooser::CREATE, "Save");
    pDoc->m_pUI->m_chooser->show();
    while(pDoc->m_pUI->m_chooser->shown())
        { Fl::wait(); }*/
	
	const char* filename = pDoc->m_pUI->fileDialog( Fl_Native_File_Chooser::BROWSE_SAVE_FILE,
													"PNG Image File (*.png)\t*.png\nJPEG Image File (*.jpg)\t*.jpg");
	if(filename) {
		std::string strFileName = (std::string)filename;
		std::string ext;
		int quality = 95;
		switch(pDoc->m_pUI->m_nativeChooser->filter_value()) {
			case 0:	ext = ".png"; break;
			case 1:	ext = ".jpg"; break;
		}
#ifdef WIN32
		char szExt[_MAX_EXT];
		_splitpath_s(strFileName.c_str(), NULL,0, NULL,0, NULL, 0, szExt,_MAX_EXT);
		if (_stricmp(szExt,".jpg") && _stricmp(szExt,".png")) {
			strFileName += ext;
		}
		else
			ext = szExt;
#endif

#ifdef WIN32
		if(!_strcmpi(ext.c_str(),".jpg")) {
#else
		if(!strcasecmp(ext.c_str(),".jpg")) {
#endif		
			Dialog2 x(0,0,0,0,"ok");
			quality = x.getValue();
		}
		pDoc->saveImage(strFileName.c_str(), ext.c_str(), quality);
	}
	/*const char *szFileName = pDoc->m_pUI->m_chooser->value();
	if (szFileName) {
		std::string strFileName = szFileName;
		std::string ext = (std::string)pDoc->m_pUI->m_chooser->filter();
		ext = ext.substr(1, ext.length() - 1);
		#ifdef WIN32
			char szExt[_MAX_EXT];
			_splitpath_s(strFileName.c_str(), NULL,0, NULL,0, NULL, 0, szExt,_MAX_EXT);
			// If user didn't type supported ext, add default one.
			if (_stricmp(szExt,".jpg") && _stricmp(szExt,".png")) {
				strFileName += ext;
			}
			else
				ext = szExt;
		#endif
		int quality = 95;
		if (!strcmp(ext.c_str(),".jpg")){
			Dialog2 x(0,0,0,0,"ok");
			quality = x.getValue();
		}
		pDoc->saveImage(strFileName.c_str(), ext.c_str(), quality);
	}*/
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
	whoami(o)->m_brushDialog->size(400, 325);
}

void ImpressionistUI::cb_filter_kernel(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	// Need to back up the current paint view status
	// so we can easily preview filters
	//If no image has been loaded, do not open filter kernel
	if (pDoc->m_ucPainting != NULL) {
		memcpy( pDoc->m_ucPreviewBackup, pDoc->m_ucPainting, 
			pDoc->m_nPaintWidth * pDoc->m_nPaintHeight * 3 );
		whoami(o)->fltDesignUI->show();
		whoami(o)->fltDesignUI->mainWindow->size(286, 340);
	}
	else
		fl_alert("Must load an image first!");
}



//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CSE 457");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;
	if (type == BRUSH_LINES || type == BRUSH_SCATTERED_LINES)
	{
		pUI->m_BrushDirectionChoice->activate();
		pUI->m_LineWidthSlider->activate();
		pUI->m_LineAngleSlider->activate();
	}
	else
	{
		pUI->m_BrushDirectionChoice->deactivate();
		pUI->m_LineWidthSlider->deactivate();
		pUI->m_LineAngleSlider->deactivate();
	}

	pDoc->setBrushType(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the alpha to use from the value of the alpha
// slider
// Called by the UI when the alpha slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_alpha = double(((Fl_Slider *)o)->value());
}

//-----------------------------------------------------------
// Updates the line width to use from the value of the line
// width slider
// Called by the UI when the line width slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_lineWidthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_lineWidth = (int)((Fl_Slider*)o)->value();
}

//-----------------------------------------------------------
// Updates the line width to use from the value of the line
// width slider
// Called by the UI when the line width slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_lineAngleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_lineAngle = (int)((Fl_Slider*)o)->value();
}

//-------------------------------------------------------------
// Sets the type of brush  direction to use to the one chosen 
// in the brush direction choice.  
// Called by the UI when a brush direction is chosen in the 
// brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushDirectionChoice(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_brushDirection = (BrushDirectionType)(int)v;
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

//-------------------------------------------------
// Gets the line width
//-------------------------------------------------
int ImpressionistUI::getLineWidth()
{
	return m_lineWidth;
}

//-------------------------------------------------
// Gets the line angle
//-------------------------------------------------
int ImpressionistUI::getLineAngle()
{
	return m_lineAngle;
}

//-------------------------------------------------
// Sets the line angle
//-------------------------------------------------
void ImpressionistUI::setLineAngle(int theta)
{
	theta %= 360;
	m_lineAngle = theta;
	if (0 <= theta && theta <= 359)
	{
		printf("yolo\n");
		m_LineAngleSlider->value(theta);
	}
}

//------------------------------------------------
// Return the alpha level
//------------------------------------------------
double ImpressionistUI::getAlpha()
{
	return m_alpha;
}

//------------------------------------------------
// Set the alpha level
//------------------------------------------------
void ImpressionistUI::setAlpha(double alpha)
{
	m_alpha = alpha;

	if (alpha >= 0.00 && alpha <= 1.00)
		m_BrushAlphaSlider->value(m_alpha);
}

//------------------------------------------------
// Gets the brush direction type
//------------------------------------------------
BrushDirectionType ImpressionistUI::getBrushDirection()
{
	return m_brushDirection;
}

// Getting/setting methods for the filter design UI
void ImpressionistUI::initFltDesignUI(void)
{
	fltDesignUI->ele0->value(fltKernel[0]);
	fltDesignUI->ele1->value(fltKernel[1]);
	fltDesignUI->ele2->value(fltKernel[2]);
	fltDesignUI->ele3->value(fltKernel[3]);
	fltDesignUI->ele4->value(fltKernel[4]);
	fltDesignUI->ele5->value(fltKernel[5]);
	fltDesignUI->ele6->value(fltKernel[6]);
	fltDesignUI->ele7->value(fltKernel[7]);
	fltDesignUI->ele8->value(fltKernel[8]);
	fltDesignUI->ele9->value(fltKernel[9]);
	fltDesignUI->ele10->value(fltKernel[10]);
	fltDesignUI->ele11->value(fltKernel[11]);
	fltDesignUI->ele12->value(fltKernel[12]);
	fltDesignUI->ele13->value(fltKernel[13]);
	fltDesignUI->ele14->value(fltKernel[14]);
	fltDesignUI->ele15->value(fltKernel[15]);
	fltDesignUI->ele16->value(fltKernel[16]);
	fltDesignUI->ele17->value(fltKernel[17]);
	fltDesignUI->ele18->value(fltKernel[18]);
	fltDesignUI->ele19->value(fltKernel[19]);
	fltDesignUI->ele20->value(fltKernel[20]);
	fltDesignUI->ele21->value(fltKernel[21]);
	fltDesignUI->ele22->value(fltKernel[22]);
	fltDesignUI->ele23->value(fltKernel[23]);
	fltDesignUI->ele24->value(fltKernel[24]);

	fltDesignUI->scale->value(scale);
	fltDesignUI->offset->value(offset);
}

void ImpressionistUI::updateFilter()
{
	fltKernel[0] = fltDesignUI->ele0->value();
	fltKernel[1] = fltDesignUI->ele1->value();
	fltKernel[2] = fltDesignUI->ele2->value();
	fltKernel[3] = fltDesignUI->ele3->value();
	fltKernel[4] = fltDesignUI->ele4->value();
	fltKernel[5] = fltDesignUI->ele5->value();
	fltKernel[6] = fltDesignUI->ele6->value();
	fltKernel[7] = fltDesignUI->ele7->value();
	fltKernel[8] = fltDesignUI->ele8->value();
	fltKernel[9] = fltDesignUI->ele9->value();
	fltKernel[10] = fltDesignUI->ele10->value();
	fltKernel[11] = fltDesignUI->ele11->value();
	fltKernel[12] = fltDesignUI->ele12->value();
	fltKernel[13] = fltDesignUI->ele13->value();
	fltKernel[14] = fltDesignUI->ele14->value();
	fltKernel[15] = fltDesignUI->ele15->value();
	fltKernel[16] = fltDesignUI->ele16->value();
	fltKernel[17] = fltDesignUI->ele17->value();
	fltKernel[18] = fltDesignUI->ele18->value();
	fltKernel[19] = fltDesignUI->ele19->value();
	fltKernel[20] = fltDesignUI->ele20->value();
	fltKernel[21] = fltDesignUI->ele21->value();
	fltKernel[22] = fltDesignUI->ele22->value();
	fltKernel[23] = fltDesignUI->ele23->value();
	fltKernel[24] = fltDesignUI->ele24->value();

	scale = fltDesignUI->scale->value();
	offset = fltDesignUI->offset->value();

//	printf("update filter\n");
}




// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Filter Kernel...", FL_CTRL + 'f', (Fl_Callback *)ImpressionistUI::cb_filter_kernel }, 
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {0}
};

Fl_Menu_Item ImpressionistUI::brushDirectionMenu[NUM_BRUSH_DIRECTION + 1] {
	{ "Slider/Right Mouse", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_brushDirectionChoice, (void *)BRUSH_DIRECTION_LINE },
	{ "Gradient", FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_brushDirectionChoice, (void *)BRUSH_DIRECTION_GRADIENT },
	{ "Brush Direction", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushDirectionChoice, (void *)BRUSH_DIRECTION_STROKE },
};

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() : m_nativeChooser(NULL) {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view", m_origView);//0jon

			// install paint view window
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// Create the filter kernel UI
	fltDesignUI = new FltDesignUI;
	fltDesignUI->impUI = this;

	// initialize the filter kernel
	for (int i=0;i<FLT_HEIGHT*FLT_WIDTH;i++)
	{
		fltKernel[i] = 0;
	}
	fltKernel[(FLT_HEIGHT/2)*FLT_WIDTH+(FLT_HEIGHT/2)]=1;
	scale = 1;
	offset = 0;

	initFltDesignUI();

	// init values

	m_nSize = 10;
	m_lineWidth = 1;
	m_alpha = 1.00;
	m_lineAngle = 0;
	m_brushDirection = BRUSH_DIRECTION_LINE;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		m_BrushDirectionChoice = new Fl_Choice(115, 45, 150, 25, "&Stroke Direction");
		m_BrushDirectionChoice->user_data((void*)(this));
		m_BrushDirectionChoice->menu(brushDirectionMenu);
		m_BrushDirectionChoice->callback(cb_brushDirectionChoice);
		m_BrushDirectionChoice->deactivate();


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Add line width slider
		m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
		m_LineWidthSlider->user_data((void*)(this));
		m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_LineWidthSlider->labelfont(FL_COURIER);
		m_LineWidthSlider->labelsize(12);
		m_LineWidthSlider->minimum(1);
		m_LineWidthSlider->maximum(40);
		m_LineWidthSlider->step(1);
		m_LineWidthSlider->value(1);
		m_LineWidthSlider->align(FL_ALIGN_RIGHT);
		m_LineWidthSlider->deactivate();
		m_LineWidthSlider->callback(cb_lineWidthSlides);

		// Add line angle slider
		m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
		m_LineAngleSlider->user_data((void*)(this));
		m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_LineAngleSlider->labelfont(FL_COURIER);
		m_LineAngleSlider->labelsize(12);
		m_LineAngleSlider->minimum(0);
		m_LineAngleSlider->maximum(359);
		m_LineAngleSlider->step(1);
		m_LineAngleSlider->value(0);
		m_LineAngleSlider->align(FL_ALIGN_RIGHT);
		m_LineAngleSlider->deactivate();
		m_LineAngleSlider->callback(cb_lineAngleSlides);

		// Add brush alpha slider
		m_BrushAlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_BrushAlphaSlider->user_data((void*)(this));
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0.00);
		m_BrushAlphaSlider->maximum(1.00);
		m_BrushAlphaSlider->step(0.01);
		m_BrushAlphaSlider->value(1.00);
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlider->callback(cb_alphaSlides);


    m_brushDialog->end();

}

ImpressionistUI::~ImpressionistUI()
{
	if(m_nativeChooser) delete m_nativeChooser;
	delete fltDesignUI;
}

void ImpressionistUI::cancelFilter(void)
{
	std::swap( m_pDoc->m_ucPainting, m_pDoc->m_ucPreviewBackup );
	fltDesignUI->hide();
	m_paintView->refresh();
}

void ImpressionistUI::applyFilter(void)
{
	m_pDoc->applyFilter(m_pDoc->m_ucPreviewBackup, 
		m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight,
		m_pDoc->m_ucPainting, 
		fltKernel,FLT_WIDTH,FLT_HEIGHT,scale,offset);
	fltDesignUI->hide();
	m_paintView->refresh();
}

void ImpressionistUI::previewFilter(void)
{
	m_pDoc->applyFilter(m_pDoc->m_ucPreviewBackup, 
		m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight,
		m_pDoc->m_ucPainting, 
		fltKernel,FLT_WIDTH,FLT_HEIGHT,scale,offset);
	m_paintView->refresh();
}



