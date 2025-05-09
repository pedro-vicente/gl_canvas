#include "wx/wxprec.h"
#include "wx/wx.h"
#include "sample.xpm"
#include <wx/glcanvas.h>

#if defined(__WXMAC__)
#ifdef __DARWIN__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <gl.h>
#  include <glu.h>
#endif
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
// GLCanvasControls
/////////////////////////////////////////////////////////////////////////////////////////////////////

class GLCanvasControls : public wxGLCanvas
{
public:
  GLCanvasControls(wxWindow* parent);
  virtual ~GLCanvasControls();
  void OnPaint(wxPaintEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnMouseEvent(wxMouseEvent& event);

private:
  wxGLContext* gl_context;
  wxDECLARE_EVENT_TABLE();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
// wxAppGLControls
/////////////////////////////////////////////////////////////////////////////////////////////////////

class wxAppGLControls : public wxApp
{
public:
  virtual bool OnInit() wxOVERRIDE;
};

wxIMPLEMENT_APP(wxAppGLControls);

/////////////////////////////////////////////////////////////////////////////////////////////////////
// wxFrameGLControls
/////////////////////////////////////////////////////////////////////////////////////////////////////

class wxFrameGLControls : public wxFrame
{
public:
  wxFrameGLControls(const wxString& title);
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  virtual ~wxFrameGLControls();

  GLCanvasControls* gl_canvas;
private:
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(wxFrameGLControls, wxFrame)
EVT_MENU(wxID_EXIT, wxFrameGLControls::OnQuit)
EVT_MENU(wxID_ABOUT, wxFrameGLControls::OnAbout)
wxEND_EVENT_TABLE()

/////////////////////////////////////////////////////////////////////////////////////////////////////
// wxAppGLControls::OnInit()
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool wxAppGLControls::OnInit()
{
  if (!wxApp::OnInit())
  {
    return false;
  }
  wxFrameGLControls* frame = new wxFrameGLControls("OpenGL controls");
  frame->Show(true);
  frame->SetSize(400, 400, 1000, 700);
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// wxFrameGLControls::wxFrameGLControls
/////////////////////////////////////////////////////////////////////////////////////////////////////

wxFrameGLControls::wxFrameGLControls(const wxString& title)
  : wxFrame(NULL, wxID_ANY, title)
{
  SetIcon(wxICON(sample));
  wxMenu* menu_file = new wxMenu;
  menu_file->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");
  wxMenu* menu_help = new wxMenu;
  menu_help->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
  wxMenuBar* menu_bar = new wxMenuBar();
  menu_bar->Append(menu_file, "&File");
  menu_bar->Append(menu_help, "&Help");
  SetMenuBar(menu_bar);
  CreateStatusBar(2);
  SetStatusText("Ready");

  gl_canvas = new GLCanvasControls(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ~wxFrameGLControls
/////////////////////////////////////////////////////////////////////////////////////////////////////

wxFrameGLControls::~wxFrameGLControls()
{
  delete gl_canvas;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameGLControls::OnQuit
/////////////////////////////////////////////////////////////////////////////////////////////////////

void wxFrameGLControls::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//wxFrameGLControls::OnAbout
/////////////////////////////////////////////////////////////////////////////////////////////////////

void wxFrameGLControls::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// GLCanvasControls
/////////////////////////////////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(GLCanvasControls, wxGLCanvas)
EVT_SIZE(GLCanvasControls::OnSize)
EVT_PAINT(GLCanvasControls::OnPaint)
EVT_MOUSE_EVENTS(GLCanvasControls::OnMouseEvent)
wxEND_EVENT_TABLE()

GLCanvasControls::GLCanvasControls(wxWindow* parent)
  : wxGLCanvas(parent)
{
  gl_context = new wxGLContext(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ~GLCanvasControls
/////////////////////////////////////////////////////////////////////////////////////////////////////

GLCanvasControls::~GLCanvasControls()
{
  delete gl_context;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// OnPaint
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GLCanvasControls::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);

  float color[3];

  color[0] = 1; //red
  color[1] = 0; //green 
  color[2] = 0; //blue

  SetCurrent(*gl_context);

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  glColor3f(color[0], color[1], color[2]);
  glVertex3f(-0.5f, -0.5f, 0.0f);
  glVertex3f(0.5f, -0.5f, 0.0f);
  glVertex3f(0.0f, 0.5f, 0.0f);
  glEnd();

  glFlush();
  SwapBuffers();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// OnSize
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GLCanvasControls::OnSize(wxSizeEvent& event)
{
  if (!IsShownOnScreen())
    return;

  //Makes the OpenGL state that is represented by the OpenGL rendering context context current, i.e.
  //it will be used by all subsequent OpenGL calls.
  //Note: This function may only be called when the window is shown on screen, 
  //particular it can't usually be called from the constructor as the window
  //isn't yet shown at this moment.
  SetCurrent(*gl_context);

  const wxSize size = event.GetSize() * GetContentScaleFactor();
  glViewport(0, 0, size.x, size.y);


}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// OnMouseEvent
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GLCanvasControls::OnMouseEvent(wxMouseEvent& event)
{
  event.Skip();
  if (event.LeftIsDown())
  {

  }
  else
  {
  }
}

