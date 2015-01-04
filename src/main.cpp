#include <window.h>
#include <glog/logging.h>


int single_buffer_attributes[] = {
    GLX_X_RENDERABLE,  True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE,      8,
    GLX_GREEN_SIZE,    8,
    GLX_BLUE_SIZE,     8,
    GLX_ALPHA_SIZE,    8,
    GLX_DEPTH_SIZE,    24,
    GLX_STENCIL_SIZE,  8,
    None
};

int double_buffer_attributes[] = {
    GLX_X_RENDERABLE,  True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,  
    GLX_RED_SIZE,      8,
    GLX_GREEN_SIZE,    8, 
    GLX_BLUE_SIZE,     8,
    GLX_ALPHA_SIZE,    8,
    GLX_DEPTH_SIZE,    24,
    GLX_STENCIL_SIZE,  8,
    GLX_DOUBLEBUFFER,  True,
    None
};

void clean();
void update();
void gl_init();
void display();
void reshape(int, int);
void key_press(window*, XKeyEvent&);
void key_release(window*, XKeyEvent&);
void button_press(window*, XButtonEvent&);
void button_release(window*, XButtonEvent&);

int main(int argc, char **argv) {
 
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = 1;   

  XInitThreads(); 
  window wnd("My window manager", double_buffer_attributes);
  try {
    wnd.init();
  } catch (char const *msg) {
    std::cout << "Catch: " << msg << std::endl;
  }    
  wnd.set_gl_init(gl_init);  
  wnd.set_key_press_event(key_press);
  wnd.set_key_release_event(key_release);
  wnd.set_mouse_button_press_event(button_press);
  wnd.set_mouse_button_release_event(button_release);
  wnd.set_reshape_event(reshape);  
  wnd.run();  
  return 0;
}

void gl_init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1., 1., -1., 1., 1., 20.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
    
  glEnable(GL_DEPTH_TEST); 
}

void display() { 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_QUADS);
    glColor3f(1., 0., 0.);
    glVertex3f(-.75, -.75, 0.);
    glColor3f(0., 1., 0.);
    glVertex3f(.75, -.75, 0.);
    glColor3f(0., 0., 1.);
    glVertex3f(-.75, -.75, 0.);
    glColor3f(1., 1., 0.);
    glVertex3f(-.75, .75, 0.);
  glEnd();
  glFlush();
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  display();
}

void key_press(window *sender, XKeyEvent &event) {
  switch(event.keycode) {
    case 0x09: {
      sender->close();
        break;
    }
    case 0x0A: {
      sender->fullscreen();
        break;
    }
    case 0x0B: {
      sender->normal();
      break;
    }
    case 0x0C: {
      sender->maximize();
      break;
    }
    case 0x0D: {
      sender->minimize();
      break;
    }
    case 0x0E: {
      sender->set_window_size(1024, 768);
      break;
    }
    case 0x0F: {
      sender->set_window_pos(200, 200);
      break;
    }
    case 0x10: {
      break;
    }
    default:         
      break;
  }
}

void key_release(window *sender, XKeyEvent &event) {
  //LOG(INFO) << "Key was released " << event.keycode 
  //          << " and state is " << event.state;   
}

void button_press(window *sender, XButtonEvent &event) {
  //LOG(INFO) << "Mouse button was pressed on x = " << event.x
  //          << "y = " << event.y;
}

void button_release(window *sender, XButtonEvent &event) {
  //LOG(INFO) << "Mouse button was released on x = " << event.x
  //          << "y = " << event.y;
}
