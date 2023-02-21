/***********************************************************************
 *                                                                     *
 * TEST.CPP                                                            *
 *   This module contains the Turbo Vision application code to run     *
 *   this example.  It sets up the necessary menus to bring up the     *
 *   test module represented by this demo.                             *
 *                                                                     *
 * TEST MODULE for Multiple Menu Bar Demo.                             *
 *                                                                     *
 ***********************************************************************
 *                                                                     *
 * This code was written by Borland Technical Support.                 *
 * It is provided as is with no warranties expressed or implied.       *
 *                                                                     *
 ***********************************************************************/

#define Uses_TRect
#define Uses_TKeys
#define Uses_TEvent
#define Uses_TDialog
#define Uses_TMenu
#define Uses_TMenuItem
#define Uses_TMenuBar
#define Uses_TDeskTop
#define Uses_TProgram
#define Uses_TApplication
#include <tvision/tv.h>

#pragma hdrstop

#if !defined( __CMDS_H )
#include "cmds.h"
#endif

#if !defined( __MMENU_H )
#include "mmenu.h"
#endif

#include <tvision/monolithic_examples.h>


/***********************************************************************
 *
 * Application object for demo.
 *
 ***********************************************************************/
class MMTestApp : public TApplication
{

public:

    MMTestApp();
    static TMenuBar *initMenuBar( TRect r );
    virtual void handleEvent( TEvent& event );

protected:

    int curMenu;

};


/***********************************************************************
 *
 * MMTestApp::MMTestApp()
 *
 * Application object contructor.
 *
 ***********************************************************************/
MMTestApp::MMTestApp() :
    TProgInit( initStatusLine, initMenuBar, initDeskTop ),
    TApplication(),
    curMenu( 0 )
{
}


/***********************************************************************
 *
 * MMTestApp::initMenuBar( TRect r )
 *
 * Build several menus and pass them in an array to the TMultiMenu
 * constructor.
 *
 ***********************************************************************/
TMenuBar *MMTestApp::initMenuBar( TRect r )
{
    r.b.y = r.a.y + 1;
    TMenu *M[] =
    {
                    /*  Menu Number One  */
        new TMenu(
          *new TMenuItem( "~N~ext menu", cmCycle, kbAltN ) +
          *new TMenuItem( "~M~enu One", kbAltM, new TMenu(
            *new TMenuItem( "~O~ne", cmOne, kbAltO ) +
            *new TMenuItem( "~T~wo", cmTwo, kbAltT ) +
            *new TMenuItem( "T~h~ree", cmThree, kbAltH )
            )) +
          *new TMenuItem( "~F~ile", kbAltF, new TMenu(
            *new TMenuItem( "~N~ew", cmNothing, kbAltN ) +
            *new TMenuItem( "~O~pen", cmNothing, kbAltO ) +
            *new TMenuItem( "~S~ave", cmNothing, kbAltS ) +
            *new TMenuItem( "S~a~ve all", cmNothing, kbAltA )
            ))
          ),
                    /*  Menu Number Two  */
        new TMenu(
          *new TMenuItem( "~N~ext menu", cmCycle, kbAltN ) +
          *new TMenuItem( "~M~enu Two", kbAltM, new TMenu(
            *new TMenuItem( "~O~ne", cmOne, kbAltO ) +
            *new TMenuItem( "~T~wo", cmTwo, kbAltT ) +
            *new TMenuItem( "T~h~ree", cmThree, kbAltH )
            )) +
          *new TMenuItem( "~E~dit", kbAltE, new TMenu(
            *new TMenuItem( "Cu~t~", cmNothing, kbAltT ) +
            *new TMenuItem( "~C~opy", cmNothing, kbAltC ) +
            *new TMenuItem( "~P~aste", cmNothing, kbAltP )
            ))
          ),
                    /*  Menu Number Three  */
        new TMenu(
          *new TMenuItem( "~N~ext menu", cmCycle, kbAltN ) +
          *new TMenuItem( "~M~enu Three", kbAltM, new TMenu(
            *new TMenuItem( "~O~ne", cmOne, kbAltO ) +
            *new TMenuItem( "~T~wo", cmTwo, kbAltT ) +
            *new TMenuItem( "T~h~ree", cmThree, kbAltH )
            )) +
          *new TMenuItem( "~C~ompile", kbAltC, new TMenu(
            *new TMenuItem( "~C~ompile", cmNothing, kbAltO ) +
            *new TMenuItem( "~M~ake", cmNothing, kbAltT ) +
            *new TMenuItem( "~L~ink", cmNothing, kbAltH ) +

            *new TMenuItem( "~B~uild All", cmNothing, kbAltH )
            ))
          ),
                    /*  END of Menu List Marker (NULL)  */
        0
    };
    return new TMultiMenu( r, M );
}


/***********************************************************************
 *
 * MMTestApp::handleEvent( TEvent& event )
 *
 * Send appropriate messages to the new multi menu object in response
 * to commands generated by the current menu.  The menu bar will switch
 * to a new setting automatically upon receiving the correct message.
 *
 ***********************************************************************/
void MMTestApp::handleEvent( TEvent& event )
{
    if( event.what == evCommand &&
        event.message.command >= cmOne &&
        event.message.command <= cmThree
      )
    {
        curMenu = (event.message.command - cmOne) % 3;

        message( TProgram::menuBar, evBroadcast, cmMMChangeMenu,
                 (void *)(size_t) curMenu
               );
        clearEvent( event );
    }
    else if( event.what == evCommand && event.message.command == cmCycle )
    {
        curMenu = (curMenu + 1) % 3;
        message( TProgram::menuBar, evBroadcast, cmMMChangeMenu,
                 (void *)(size_t) curMenu
               );
        clearEvent( event );
    }
    else
        TApplication::handleEvent( event );
}


/***********************************************************************
 *
 * main()
 *
 ***********************************************************************/


#if defined(BUILD_MONOLITHIC)
#define main    tvision_mmenu_main
#endif

int main()
{
    MMTestApp TB;
    TB.run();
    return 0;
}
