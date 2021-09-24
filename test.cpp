#include <Performer/pf.h>

main() {
  //Init OpenGL Performer
  void pfInit(void);

  pfConfig();

  pfPipe *pipe = pfGetPipe(0);

  pfPipeWindow *pwin0 = pfNewPWin(pipe);

  pfChannel *chan0 = pfNewChan(pipe);

  pfOpenPWin(pwin0);
  pfFrame();

  void pfFilePath(const char *pathName);

  void pfChanFOV(pfChannel* chan, float horiz, float vert);
  void pfChanNearFar(pfChannel* chan, float near, float far);

  void pfFrame(void);
  
}
