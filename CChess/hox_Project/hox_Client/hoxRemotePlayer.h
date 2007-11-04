/////////////////////////////////////////////////////////////////////////////
// Name:            hoxRemotePlayer.h
// Program's Name:  Huy's Open Xiangqi
// Created:         11/01/2007
//
// Description:     The REMOTE Player.
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_HOX_REMOTE_PLAYER_H_
#define __INCLUDED_HOX_REMOTE_PLAYER_H_

#include <wx/wx.h>
#include <wx/socket.h>
#include "hoxPlayer.h"
#include "hoxEnums.h"
#include "hoxTypes.h"

/* Forward declarations */
class hoxServer;

/**
 * The REMOTE player.
 */

class hoxRemotePlayer :  public hoxPlayer
{
  public:
    hoxRemotePlayer(); // DUMMY default constructor required for event handler.
    hoxRemotePlayer( const wxString& name,
                     hoxPlayerType   type,
                     int             score );

    virtual ~hoxRemotePlayer();

    /*******************************************
     * Override the parent's event-handler API
     *******************************************/

    virtual void OnNewMove_FromTable( hoxPlayerEvent&  event );
    virtual void OnWallMsg_FromTable( wxCommandEvent&  event );

    /***************************
     * Network API
     ***************************/

    void OnIncomingNetworkData( wxSocketEvent& event );

    hoxResult     SetCBSocket( wxSocketBase* socket );

    void SetServer( hoxServer* server) { m_server = server; }

  private:

    wxSocketBase*     m_pCBSock; 
        /* Callback socket for only NETWORK player.  */

    hoxServer*        m_server;

    DECLARE_CLASS(hoxRemotePlayer)
    DECLARE_EVENT_TABLE()
};

#endif /* __INCLUDED_HOX_REMOTE_PLAYER_H_ */