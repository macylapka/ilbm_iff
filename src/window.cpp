#include <window.h>

bool window::ctx_error_ocurred = false;

int window::ctx_error_handler(Display *dpy, XErrorEvent *ev) {
  ctx_error_ocurred = true;
  return 0;
}

static bool isExtensionSupported(const char *extList, const char *extension) {
  const char *start;
  const char *where, *terminator;
 
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;
 
  /* It takes a bit of care to be fool-proof about parsing the
   * OpenGL extensions string. Don't be fooled by sub-strings,
   * etc.                                                       
   */
  for (start=extList;;) {
    where = strstr(start, extension);
 
    if (!where)
      break;
 
    terminator = where + strlen(extension);
 
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0')
        return true;
 
    start = terminator;
  }
 
  return false;
}

window::~window() {

}

window::window(std::string new_title, int *new_attributes) {
  running = false;
  swap_flag = false;  
  this->title = new_title;
  is_wnd_maximized_horz = false;
  is_wnd_maximized_vert = false;
  is_wnd_fullscreen = false;
  dpy = NULL;
  visual_info = NULL;
  attributes = new_attributes;
}

void window::fullscreen() {
  XEvent wnd_event; 
  wnd_event.type = ClientMessage;
  wnd_event.xclient.window = wnd;
  wnd_event.xclient.message_type = wm_state;
  wnd_event.xclient.format = 32;
  wnd_event.xclient.data.l[0] = 1;
  wnd_event.xclient.data.l[1] = wm_fullscreen;
  wnd_event.xclient.data.l[2] = 0;
  XSendEvent(dpy, DefaultRootWindow(dpy), False, 
    SubstructureRedirectMask | SubstructureNotifyMask, &wnd_event);
}

void window::maximize() {
  get_window_states();
  
  XEvent wnd_event;
  wnd_event.type = ClientMessage;
  wnd_event.xclient.window = wnd;
  wnd_event.xclient.message_type = wm_state;
  wnd_event.xclient.format = 32;
  if(is_wnd_fullscreen) {
      wnd_event.xclient.data.l[0] = 0;
      wnd_event.xclient.data.l[1] = wm_fullscreen;
      wnd_event.xclient.data.l[2] = 0; 
  } 
  XSendEvent(dpy, DefaultRootWindow(dpy), False, 
      SubstructureRedirectMask | SubstructureNotifyMask, &wnd_event);
  
  wnd_event.type = ClientMessage;
  wnd_event.xclient.window = wnd;
  wnd_event.xclient.message_type = wm_state;
  wnd_event.xclient.format = 32;
  // _NET_WM_STATE_REMOVE   0   // remove/unset property
  // _NET_WM_STATE_ADD      1   // add/set property
  // _NET_WM_STATE_TOGGLE   2   // toggle property
  wnd_event.xclient.data.l[0] = 1;
  wnd_event.xclient.data.l[1] = wm_maximize_window_horz;
  wnd_event.xclient.data.l[2] = wm_maximize_window_vert;
  XSendEvent(dpy, DefaultRootWindow(dpy), False, 
    SubstructureRedirectMask | SubstructureNotifyMask, &wnd_event);  
}

void window::minimize() {
  XEvent wnd_event;
  wnd_event.type = ClientMessage;
  wnd_event.xclient.window = wnd;
  wnd_event.xclient.message_type = wm_change_state;
  wnd_event.xclient.format = 32;
  //WITHDRAWN_STATE   0
  //NORMAL_STATE      1
  //ICONIC_STATE      3
  wnd_event.xclient.data.l[0] = 3; 
  XSendEvent(dpy, DefaultRootWindow(dpy), False, 
    SubstructureRedirectMask | SubstructureNotifyMask, &wnd_event); 
}

void window::normal() { 
  get_window_states();
  XEvent wnd_event;
  wnd_event.type = ClientMessage;
  wnd_event.xclient.window = wnd;
  wnd_event.xclient.message_type = wm_state;
  wnd_event.xclient.format = 32;
  if(is_wnd_fullscreen) {
      wnd_event.xclient.data.l[0] = 0;
      wnd_event.xclient.data.l[1] = wm_fullscreen;
      wnd_event.xclient.data.l[2] = 0; 
  } 
  XSendEvent(dpy, DefaultRootWindow(dpy), False, 
      SubstructureRedirectMask | SubstructureNotifyMask, &wnd_event); 
  
  if(is_wnd_maximized_horz) {
      wnd_event.xclient.data.l[0] = 0;
      wnd_event.xclient.data.l[1] = wm_maximize_window_horz;
      wnd_event.xclient.data.l[2] = 0;
      wnd_event.xclient.data.l[3] = 0; 
  }      
  XSendEvent(dpy, DefaultRootWindow(dpy), False, 
      SubstructureRedirectMask | SubstructureNotifyMask, &wnd_event);   

  if(is_wnd_maximized_vert) {
      wnd_event.xclient.data.l[0] = 0;
      wnd_event.xclient.data.l[1] = wm_maximize_window_vert;
      wnd_event.xclient.data.l[2] = 0;
      wnd_event.xclient.data.l[3] = 0; 
  }      
  XSendEvent(dpy, DefaultRootWindow(dpy), False, 
      SubstructureRedirectMask | SubstructureNotifyMask, &wnd_event);

}

void window::close() {
  XEvent wnd_event;
  wnd_event.type = ClientMessage;
  wnd_event.xclient.window = wnd;
  wnd_event.xclient.message_type = wm_close_window;
  wnd_event.xclient.format = 32;
  wnd_event.xclient.data.l[0] = CurrentTime;
  wnd_event.xclient.data.l[1] = 1;
  XSendEvent(dpy, DefaultRootWindow(dpy), False, SubstructureRedirectMask | 
    SubstructureNotifyMask, &wnd_event);
}

void window::cleanup() {
  if(dpy != NULL) {  
    glXMakeCurrent(dpy, None, NULL);    
    glXDestroyContext(dpy, glx_context);  
    XDestroyWindow(dpy, wnd);  
    XFreeColormap(dpy, color_map);
  }
}

void window::init() {
  dpy = XOpenDisplay(NULL);
  if(!dpy) {
    throw "Faild to open X display";
  }

  wm_change_state = XInternAtom(dpy, "WM_CHANGE_STATE", False);
  wm_delete_message = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  wm_state = XInternAtom(dpy, "_NET_WM_STATE", False); 
  wm_close_window = XInternAtom(dpy, "_NET_CLOSE_WINDOW", False); 
  wm_maximize_window_horz = XInternAtom(dpy, 
    "_NET_WM_STATE_MAXIMIZED_HORZ", False);
  wm_maximize_window_vert = XInternAtom(dpy,
    "_NET_WM_STATE_MAXIMIZED_VERT", False);
  wm_fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
  wm_move_resize = XInternAtom(dpy, "_NET_MOVERESIZE_WINDOW", False);  

  
  if(!glXQueryVersion(dpy, &glx_major, &glx_minor) || 
      ((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1)) {
    throw "Invalid GLX version";
  }
#ifdef DEBUG
  std::cout << "glx version = " << glx_major << "." << glx_minor << std::endl;
#endif

  int fb_number; 
  fb_configs = glXChooseFBConfig(dpy, DefaultScreen(dpy), attributes, &fb_number);
  if(!fb_configs) {
    throw "Faild to retrieve a framebuffer config";
  }

#ifdef DEBUG
    std::cout << "Double buffer: ";
#endif
  if(is_double_buffer(attributes)) {
    swap_flag = true;
  }
#ifdef DEBUG
  std::cout << std::boolalpha << swap_flag
    << std::dec << std::endl; 
  std::cout << "Found " << fb_number << " matching FB configs" << std::endl;
  std::cout << "Getting XVisualInfos" << std::endl;
#endif

  int best_fb_config = -1, worst_fb_config = -1, best_num_samp = -1,
    worst_num_samp = 999;
  
  for(int i = 0; i < fb_number; ++i) {
    XVisualInfo *vi = glXGetVisualFromFBConfig(dpy, fb_configs[i]);  
    if(vi) {
      int samp_buf, samples;
      glXGetFBConfigAttrib(dpy, fb_configs[i], GLX_SAMPLE_BUFFERS, &samp_buf);
      glXGetFBConfigAttrib(dpy, fb_configs[i], GLX_SAMPLES, &samples);

#ifdef DEBUG
      std::cout << "  Matching fb_config " << i << " visual ID 0x" << std::hex << vi->visualid
        << std::dec << ": SAMPLE_BUFFERS = " << samp_buf << ", SAMPLES = " << samples 
        << std::endl;
#endif

      if((best_fb_config < 0 || samp_buf) && samples > best_num_samp && swap_flag)
        best_fb_config = i, best_num_samp = samples;

      if(samp_buf == 0 && samples > best_num_samp && !swap_flag)
        best_fb_config = i, best_num_samp = samples;
      
      if(worst_fb_config < 0 || !samp_buf || samples < worst_num_samp)
        worst_fb_config = i, worst_num_samp = samples;
    }
    XFree(vi);
  }
  
  GLXFBConfig best_fbc = fb_configs[best_fb_config];
  
  XFree(fb_configs);
  
  visual_info = glXGetVisualFromFBConfig(dpy, best_fbc);
  
#ifdef DEBUG
  std::cout << "Chosen visual ID 0x" << std::hex << visual_info->visualid 
    << std::dec << std::endl;
#endif
  XSetWindowAttributes set_wnd_attr;
  set_wnd_attr.background_pixmap = None;
  set_wnd_attr.border_pixel = 0;
  set_wnd_attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | 
    ButtonPressMask | ButtonReleaseMask | ConfigureNotify | StructureNotifyMask;
#ifdef DEBUG
  std::cout << "Creating colormap: ";
#endif
  set_wnd_attr.colormap = color_map = XCreateColormap(dpy, 
    RootWindow(dpy, visual_info->screen), visual_info->visual, AllocNone);
#ifdef DEBUG 
  std::cout << "OK" << std::endl;
#endif
 
  int swa_mask = CWBorderPixel | CWColormap | CWEventMask;

#ifdef DEBUG
  std::cout << "Creating window: ";
#endif
  wnd = XCreateWindow(dpy, RootWindow(dpy, visual_info->screen), 0, 0, 640, 512, 0, 
    visual_info->depth, InputOutput, visual_info->visual, 
    swa_mask, &set_wnd_attr);
#ifdef DEBUG
  std::cout << "OK" << std::endl;
#endif

  if(!wnd) {
    throw "Failed to create window";
  }
  XFree(visual_info);

  XStoreName(dpy, wnd, "GL 3.0 Window");

#ifdef DEBUG
  std::cout << "Mapping window: ";
#endif
  XMapWindow(dpy, wnd);
#ifdef DEBUG
  std::cout << "OK" << std::endl;
#endif

  char const *glx_exts = glXQueryExtensionsString(dpy, DefaultScreen(dpy)); 
  
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
  glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
    glXGetProcAddressARB((GLubyte const*)"glXCreateContextAttribsARB");

  glx_context = 0;
  ctx_error_ocurred = false;
  int (*old_handler)(Display*, XErrorEvent*) =
    XSetErrorHandler(&window::ctx_error_handler);

  if(!isExtensionSupported(glx_exts, "GLX_ARB_create_context") ||
    !glXCreateContextAttribsARB) {
#ifdef DEBUG 
    std::cout << "glXCreateContextAttribsARB() not found"
      << "... using old-style GLX context" << std::endl;
#endif
    glx_context = glXCreateNewContext(dpy, best_fbc, GLX_RGBA_TYPE, NULL, True);
  } else {
    int context_attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
      GLX_CONTEXT_MINOR_VERSION_ARB, 0,
      None
    };
#ifdef DEBUG
    std::cout << "Creating context: ";
#endif
    glx_context = glXCreateContextAttribsARB(dpy, best_fbc, NULL, True, context_attribs);
#ifdef DEBUG
    std::cout << "OK" << std::endl;
#endif

    XSync(dpy, False);
    if(!ctx_error_ocurred && glx_context) {
#ifdef DEBUG
      std::cout << "Created GL 3.0 context" << std::endl;
#endif
    } else {
      context_attribs[1] = 1;
      context_attribs[3] = 0;
      ctx_error_ocurred = false; 
#ifdef DEBUG
      std::cout << "Failed to create GL 3.0 context"
        << "... using old-style GLX context" << std::endl;
#endif
      glx_context = glXCreateContextAttribsARB(dpy, best_fbc, NULL, True, context_attribs);  
    } 
  }

  XSync(dpy, False);
  XSetErrorHandler(old_handler);

  if(ctx_error_ocurred || !glx_context) {
    throw "Faild to create an OpenGL context";
  }

#ifdef DEBUG
    std::cout << "GLX rendering context obtained: ";
#endif

  if(glXIsDirect(dpy, glx_context)) {
#ifdef DEBUG
    std::cout << "Indirect" << std::endl;
#endif
  } else {
#ifdef DEBUG
    std::cout << "Direct" << std::endl;
#endif 
  }

#ifdef DEBUG 
  std::cout << "Making context current: ";
#endif
  glXMakeCurrent(dpy, wnd, glx_context);
#ifdef DEBUG
  std::cout << "OK" << std::endl;
  std::cout << "Error ocurre: " << std::boolalpha << (glGetError() != GL_NO_ERROR) 
    << std::dec << std::endl;
#endif
  
}

Display *window::get_display() {
  return dpy;
}

Window window::get_window() {
  return wnd;
}

XVisualInfo *window::get_visual_info() {
  return visual_info;
}

Colormap window::get_color_map() {
  return color_map;
}

void window::set_gl_init(void (*new_gl_init)()) {
  gl_init = new_gl_init;
}

void window::set_window_size(int width, int height) {
  XEvent wnd_event;
  wnd_event.type = ClientMessage;
  wnd_event.xclient.display = dpy;
  wnd_event.xclient.window = wnd;
  wnd_event.xclient.message_type = wm_move_resize;
  wnd_event.xclient.format = 32;
  wnd_event.xclient.data.l[0] = WIDTH | HEIGHT;
  wnd_event.xclient.data.l[1] = 0;
  wnd_event.xclient.data.l[2] = 0;
  wnd_event.xclient.data.l[3] = width;
  wnd_event.xclient.data.l[4] = height;
  XSendEvent(dpy, DefaultRootWindow(dpy), False, SubstructureRedirectMask | 
    SubstructureNotifyMask, &wnd_event);
}

void window::set_window_pos(int x, int y) {
  XEvent wnd_event;
  wnd_event.type = ClientMessage;
  wnd_event.xclient.display = dpy;
  wnd_event.xclient.window = wnd; 
  wnd_event.xclient.message_type = wm_move_resize;
  wnd_event.xclient.format = 32;
  wnd_event.xclient.data.l[0] = X | Y;
  wnd_event.xclient.data.l[1] = x;
  wnd_event.xclient.data.l[2] = y;
  wnd_event.xclient.data.l[3] = 0;
  wnd_event.xclient.data.l[4] = 0;
  XSendEvent(dpy, DefaultRootWindow(dpy), False, SubstructureRedirectMask | 
    SubstructureNotifyMask, &wnd_event);
}

void window::set_reshape_event(void (*new_reshape)(int, int)) {
  reshape = new_reshape;
}

void window::set_key_press_event(void (*new_key_press)
    (window *wnd, XKeyEvent &key_event)) {
  key_press = new_key_press;
}

void window::set_key_release_event(void (*new_key_release)
    (window *wnd, XKeyEvent &key_event)) {
  key_release = new_key_release;
}

void window::set_mouse_button_press_event(void (*new_mouse_button_press)
    (window *wnd, XButtonEvent &button_event)) {
  mouse_button_press = new_mouse_button_press;
}

void window::set_mouse_button_release_event(void (*new_mouse_button_release)
    (window *wnd, XButtonEvent &button_event)) {
  mouse_button_release = new_mouse_button_release;
}

bool window::is_maximized() {
  get_window_states();
  return is_wnd_maximized_horz & is_wnd_maximized_vert;
}

bool window::is_maximized_vert() {
  get_window_states();
  return is_wnd_maximized_vert;
}

bool window::is_maximized_horz() {
  get_window_states();
  return is_wnd_maximized_horz;
}

bool window::is_fullscreen() {
  return is_wnd_fullscreen;
}

bool window::is_running() {
  return running;
}

void window::get_window_states() {
  Atom type;
  int format;
  long unsigned item_number, bytes_after;
  char unsigned *data = NULL;
  
  XGetWindowProperty(dpy, wnd, wm_state, 0, (~0L), 
      false, AnyPropertyType, &type, &format, &item_number, 
      &bytes_after, &data);
  
  is_wnd_fullscreen = false;  
  is_wnd_maximized_horz = false;
  is_wnd_maximized_vert = false;

  for(int unsigned i = 0; i < item_number; ++i) {
    int state = reinterpret_cast<long unsigned*>(data)[i]; 

    if(state == FULL_SCREEN) { 
      is_wnd_fullscreen = true;
      continue;
    }

    if(state == MAXIMIZED_HORIZONTAL) {
      is_wnd_maximized_horz = true; 
      continue;
    }

    if(state == MAXIMIZED_VERTICAL)
      is_wnd_maximized_vert = true;    
  }
  XFree(data);
}

void window::get_display_resolutions() {
  
  XF86VidModeModeInfo **modes;
  int nr_modes;
 
  XF86VidModeGetAllModeLines(dpy, DefaultScreen(dpy), &nr_modes, &modes);
  for (int i = 0; i < nr_modes; ++i) {
    printf("%dx%d\n", modes[i]->hdisplay, modes[i]->vdisplay);
  }
  XFree(modes);
  return; 
}

void window::update() {
  XWindowAttributes win;
  XGetWindowAttributes(dpy, wnd, &win);
  reshape(win.width, win.height);
  if(swap_flag)  
    glXSwapBuffers(dpy, wnd);
}

bool window::is_double_buffer(int *attr) {
  for(int i = 0; attr[i] != None; ++i)
    if(attr[i] == GLX_DOUBLEBUFFER && 
      attr[i + 1] == True)
      return true;
  return false;
}

void window::run() {
  gl_init();
  XSetWMProtocols(dpy, wnd, &wm_delete_message, 1);
  running = true;
  get_display_resolutions(); 
  while(running) {
    XNextEvent(dpy, &event);
    switch(event.type) {
      case Expose:
        update();               
        break;
      case ClientMessage: {
        if((int unsigned)event.xclient.data.l[0] == 
            wm_delete_message) {
          running = false;
          cleanup();  
        }
        break;
      }
      case KeyPress: {
        key_press(this, event.xkey);
        break;
      }
      case KeyRelease: {
        key_release(this, event.xkey);
        break;
      }
      case ButtonPress: {
        mouse_button_press(this, event.xbutton); 
        break;
      }
      case ButtonRelease: {
        mouse_button_release(this, event.xbutton);
        break;
      }
      default:
        break;
    }
  }
  XCloseDisplay(dpy);  
}
