#include <Performer/pf.h>



main()
{
  int i;
  int NumPipes = 1;
/* Initialize IRIS Performer */
pfInit();
    /* Set number of pfPipes desired -- THIS MUST BE DONE

     * BEFORE CALLING pfConfig().

     */

    pfMultipipe(NumPipes);
/* set multiprocessing mode */

pfMultiprocess(ProcSplit);

    /* Configure IRIS Performer and fork extra processes if

     * configured for multiprocessing.

     */

    pfConfig();

    /* Optional custom mapping of pipes to screens.
* This is actually the reverse as the default.

*/
for (i=0; i < NumPipes; i++)
 pfPipeScreen(pfGetPipe(i), NumPipes-(i+1));
    {
/* set up optional DRAW pipe stage config callback */

      pfStageConfigFunc(-1,PFPROC_DRAW,ConfigPipeDraw);   
      pfConfigStage(i, PFPROC_DRAW);
}
 

    InitScene();

    InitPipe();

    InitChannel();
    pfFrame();
    /* Application main loop */
    while(!SimDone())

    {
    }

}

void ConfigPipeDraw(int pipe, uint stage){
  pfPipe *p = pfGetPipe(pipe);
  pfNotify(PFNFY_INFO, PFNFY_PRINT,"Initializing stage 0x%x of pipe %d", stage, pipe);
}

void InitChannel(void)
{

    pfChannel *chan;

    chan = pfNewChan(pfGetPipe(0));
    /* Set the callback routines for the pfChannel */
    pfChanTravFunc(chan, PFTRAV_CULL, CullFunc);

    pfChanTravFunc(chan, PFTRAV_DRAW, DrawFunc);
    /* Attach the visual database to the channel */
    pfChanScene(chan, ViewState->scene);
    /* Attach the EarthSky model to the channel */
    pfChanESky(chan, ViewState->eSky);
    /* Initialize the near and far clipping planes */
    pfChanNearFar(chan, ViewState->near, ViewState->far);
    /* Vertical FOV is matched to window aspect ratio. */
    pfChanFOV(chan, 45.0f/NumChans, -1.0f);
    /* Initialize the viewing position and direction */
    pfChanView(chan, ViewState->initView.xyz,

               ViewState->initView.hpr);

}

/* CULL PROCESS CALLBACK FOR CHANNEL*/
/* The cull function callback.  Any work that needs to be

 * done in the cull process should happen in this function.

 */

void CullFunc(pfChannel * chan, void *data)

{

    static long first = 1;
    if (first)
    {

        if ((pfGetMultiprocess() & PFMP_FORK_CULL) &&

            (ViewState->procLock & PFMP_FORK_CULL))

                pfuLockDownCull(pfGetChanPipe(chan));

        first = 0;

    }

    PreCull(chan, data);
    pfCull();               /* Cull to the viewing frustum */

    PostCull(chan, data);
}
/* DRAW PROCESS CALLBACK FOR CHANNEL*/
/* The draw function callback. Any graphics functionality
* outside IRIS Performer must be done here. I/O with pure * IRIS GL
devices must happen here.

 */

void DrawFunc(pfChannel *chan, void *data)

{

    PreDraw(chan, data);     /* Clear the viewport, etc. */
    pfDraw();                /* Render the frame */

    /* draw HUD, read IRIS GL devices, or whatever else needs
     * to be done post-draw.

     */

    PostDraw(chan, data);

}
