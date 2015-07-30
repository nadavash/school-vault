//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//
#include <cmath>

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintView.h"
#include "impBrush.h"


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static ImpBrush::Point	coord;
static ImpBrush::Point  lastCoord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l,
					 OriginalView*  origView)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	m_origView		= origView;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

	if(!valid())
	{


		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		int	cx	= m_nWindowWidth = w();
		int	cy	= m_nWindowHeight = h();



		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );

		glViewport( 0, 0, cx, cy );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D( 0, cx, 0, cy );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		// This translation is needed to make the image pixels line up with
		// the screen's pixels.  For details, visit:
		// http://www.opengl.org/resources/faq/technical/transformations.htm#tran0030
		glTranslatef( 0.375, 0.375, 0.0 );


		glClear( GL_COLOR_BUFFER_BIT );
	}

	ImpBrush::Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		ImpBrush::Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		ImpBrush::Point target( coord.x, m_nWindowHeight - coord.y );
		
		float xDiff = 0.f;
		float yDiff = 0.f;
		int curY = 0;

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			SaveCurrentBackContent();
			RestoreContent();
			break;
		case LEFT_MOUSE_DRAG:
			curY = target.y - (m_nWindowHeight - m_nDrawHeight);
			if (m_nStartCol <= target.x && target.x <= m_nEndCol &&
				m_nStartRow <= curY && curY <= m_nEndRow)
			{
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
			}
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentBackContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			lastCoord = target;
			break;
		case RIGHT_MOUSE_DRAG:
			RestoreContent();
			drawLine(lastCoord, target);
			break;
		case RIGHT_MOUSE_UP:
			RestoreContent();
			xDiff = target.x - lastCoord.x;
			yDiff = target.y - lastCoord.y;
			m_pDoc->m_pUI->setLineAngle(atan2Degrees(yDiff, xDiff) + 360);
			m_pDoc->m_pUI->setSize(sqrt(xDiff * xDiff + yDiff * yDiff));
			break;
		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA
}


int PaintView::handle(int event)
{
	ImpBrush::Point cursor(coord.x, (m_nWindowHeight) - coord.y);
	m_origView->cursor(cursor);
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell OpenGL to read pixels from the front buffer
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}

void PaintView::SaveCurrentBackContent()
{
	glReadBuffer(GL_BACK);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );

	glReadBuffer(GL_FRONT);
}

void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::drawLine(ImpBrush::Point src, ImpBrush::Point dst)
{
	glBegin(GL_LINES);
		glColor3f(1.f, 0, 0);
		glVertex2f(src.x, src.y);
		glVertex2f(dst.x, dst.y);
	glEnd();
}
