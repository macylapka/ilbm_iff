#include <window.h>
#include <ilbm_iff.h>
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

std::string path = "./samples/pictures/Jetsons_rosie.ilbm";
ilbm_iff ilbm_image(path);

void gl_init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-320., 320., -256., 256., 0., 20.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
    
  glEnable(GL_DEPTH_TEST);

  GLuint texid; 
  glGenTextures(1, &texid); 
  glBindTexture(GL_TEXTURE_2D, texid);
  glPixelStorei(GL_PACK_ALIGNMENT, 8); 
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
    ilbm_image.get_width(), ilbm_image.get_height(), 0,
    GL_RGB, GL_UNSIGNED_BYTE, ilbm_image.convert_to_rgb()); 
}

void display() { 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0); 
    glVertex3f(-320.0, 256.0, .0);
    glTexCoord2f(0, 1); 
    glVertex3f(320.0, 256.0, .0);
    glTexCoord2f(1, 1); 
    glVertex3f(320.0, -256.0, .0);
    glTexCoord2f(1, 0); 
    glVertex3f(-320.0, -256.0, .0);
  glEnd();
  glRotatef(180.0f, 0.f, 0.f, 1.f);
  glScalef(1.0f, 1.0f, -10.0f); 
  glFlush();
  glDisable(GL_TEXTURE_2D);
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
      sender->set_window_size(640, 512);
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
  LOG(INFO) << "Key was released " << event.keycode 
            << " and state is " << event.state;   
}

void button_press(window *sender, XButtonEvent &event) {
  LOG(INFO) << "Mouse button was pressed on x = " << event.x
            << "y = " << event.y;
}

void button_release(window *sender, XButtonEvent &event) {
  LOG(INFO) << "Mouse button was released on x = " << event.x
            << "y = " << event.y;
}


int main(int argc, char **argv) {
 
  google::InitGoogleLogging("Test");
  //FLAGS_logtostderr = 1;    

  XInitThreads(); 
  window wnd("My window manager", single_buffer_attributes);
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

