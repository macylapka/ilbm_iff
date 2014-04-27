#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>
#include <string.h>
#include <iostream>

#define DEFAULT     0
#define NORTH_WEST  1
#define NORTH       2
#define NORTH_EAST  3
#define WEST        4
#define CENTER      5
#define EAST        6
#define SOUTH_WEST  7
#define SOUTH       8
#define SOUTH_EAST  9
#define STATIC      10

#define X           (1 << 8)
#define Y           (1 << 9)
#define WIDTH       (1 << 10)
#define HEIGHT      (1 << 11)

#define FULL_SCREEN           320
#define MAXIMIZED_HORIZONTAL  324
#define MAXIMIZED_VERTICAL    323

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

typedef GLXContext (*glXCreateContextAttribsARBProc)
  (Display*, GLXFBConfig, GLXContext, Bool, const int*);

class window {
public:
  virtual ~window();
  window(std::string, int*);
 
  void fullscreen();
  void maximize();
  void minimize();
  void normal();
  void close();
  void cleanup();
   
  void run(); 
  void init();
  
  Display       *get_display();
  Window         get_window();
  XVisualInfo   *get_visual_info();
  Colormap       get_color_map();
  GLXFBConfig   *get_fb_configs();
  GLXContext     get_glx_context();  
  void           get_display_resolutions();

  void set_gl_init(void (*)());
  void set_window_size(int width, int height);
  void set_window_pos(int x, int y);
  void set_reshape_event(void (*)(int, int));
  void set_key_press_event(void (*)(window*, XKeyEvent&));
  void set_key_release_event(void (*)(window*, XKeyEvent&));
  void set_mouse_button_press_event(void (*)(window*, XButtonEvent&));
  void set_mouse_button_release_event(void (*)(window*, XButtonEvent&));

  bool is_maximized();
  bool is_maximized_vert();
  bool is_maximized_horz();
  bool is_fullscreen();
  bool is_running();

protected: 
  void get_window_states();
  void update();
  bool is_double_buffer(int*);
  int static ctx_error_handler(Display*, XErrorEvent*);

private:
  Display       *dpy;
  Window         wnd;
  XVisualInfo   *visual_info;
  Colormap       color_map;
  XEvent         event; 
  
  GLXContext    glx_context;    
  GLXFBConfig  *fb_configs; 
  
  std::string title;

  int *attributes;
  int glx_major;
  int glx_minor;
 
  bool static ctx_error_ocurred;
  bool is_wnd_maximized_horz;
  bool is_wnd_maximized_vert;
  bool is_wnd_fullscreen; 
  bool need_update;
  bool running;   
  bool swap_flag;

  Atom wm_change_state;
  Atom wm_delete_message;
  Atom wm_state; 
  Atom wm_close_window;
  Atom wm_maximize_window_horz;
  Atom wm_maximize_window_vert;
  Atom wm_fullscreen;
  Atom wm_move_resize; 

  void (*gl_init)();  
  void (*reshape)(int, int); 
  void (*key_press)(window*, XKeyEvent&);
  void (*key_release)(window*, XKeyEvent&);
  void (*mouse_button_press)(window*, XButtonEvent&);
  void (*mouse_button_release)(window*, XButtonEvent&);
};
