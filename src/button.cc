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
  void OnMouseUp(wxMouseEvent& event);

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // texture
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  void load_texture(const std::string& filename);
  GLuint texture_id;
  float button_x;
  float button_y;
  float button_width;
  float button_height;

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

  // ensure wxImage PNG handler is initialized
  wxImage::AddHandler(new wxPNGHandler);

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
wxEND_EVENT_TABLE()

GLCanvasControls::GLCanvasControls(wxWindow* parent)
  : wxGLCanvas(parent), texture_id(0), button_x(0.0f), button_y(0.0f),
  button_width(0.2f), button_height(0.4f)
{
  gl_context = new wxGLContext(this);

  Bind(wxEVT_LEFT_UP, &GLCanvasControls::OnMouseUp, this);

  load_texture("button_122.png");
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
  SetCurrent(*gl_context);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLfloat x;
  GLfloat y;

  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 1.0f);
  x = button_x - button_width / 2.0f;
  y = button_y - button_height / 2.0f;
  glVertex2f(x, y);

  glTexCoord2f(1.0f, 1.0f);
  x = button_x + button_width / 2.0f;
  y = button_y - button_height / 2.0f;
  glVertex2f(x, y);

  glTexCoord2f(1.0f, 0.0f);
  x = button_x + button_width / 2.0f;
  y = button_y + button_height / 2.0f;
  glVertex2f(x, y);

  glTexCoord2f(0.0f, 0.0f);
  x = button_x - button_width / 2.0f;
  y = button_y + button_height / 2.0f;
  glVertex2f(x, y);

  glEnd();

  glDisable(GL_TEXTURE_2D);
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// load_texture
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GLCanvasControls::load_texture(const std::string& filename)
{
  SetCurrent(*gl_context);

  wxImage image;
  bool load = false;

  if (wxFileExists(filename))
  {
    load = image.LoadFile(filename);
  }

  if (image.GetWidth() != 122 || image.GetHeight() != 122)
  {
    image.Rescale(122, 122, wxIMAGE_QUALITY_HIGH);
  }

  if (!image.HasAlpha())
  {
    image.InitAlpha();
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // generate texture
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // convert image data to the format needed by OpenGL, RGBA data from RGB and alpha
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  unsigned char* data_image = image.GetData();
  unsigned char* data_alpha = image.GetAlpha();

  std::vector<unsigned char> data_rgba(122 * 122 * 4);
  for (int i = 0, j = 0; i < 122 * 122 * 3; i += 3, j += 4)
  {
    data_rgba[j] = data_image[i];            // R
    data_rgba[j + 1] = data_image[i + 1];    // G
    data_rgba[j + 2] = data_image[i + 2];    // B
    data_rgba[j + 3] = data_alpha[i / 3];    // A
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 122, 122, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_rgba.data());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// OnMouseUp
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GLCanvasControls::OnMouseUp(wxMouseEvent& event)
{
  wxPoint pos = event.GetPosition();

  //normalized coordinates
  float x = 2.0f * pos.x / GetSize().x - 1.0f;
  float y = -(2.0f * pos.y / GetSize().y - 1.0f);

  if (x >= button_x - button_width / 2.0f &&
    x <= button_x + button_width / 2.0f &&
    y >= button_y - button_height / 2.0f &&
    y <= button_y + button_height / 2.0f)
  {
    wxLogDebug("%f, %f", x, y);
  }


  Refresh();
}