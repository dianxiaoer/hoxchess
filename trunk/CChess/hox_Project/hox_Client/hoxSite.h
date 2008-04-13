/***************************************************************************
 *  Copyright 2007, 2008 Huy Phan  <huyphan@playxiangqi.com>               *
 *                                                                         * 
 *  This file is part of HOXChess.                                         *
 *                                                                         *
 *  HOXChess is free software: you can redistribute it and/or modify       *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  HOXChess is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with HOXChess.  If not, see <http://www.gnu.org/licenses/>.      *
 ***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Name:            hoxSite.h
// Created:         11/24/2007
//
// Description:     The Site.
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_HOX_SITE_H_
#define __INCLUDED_HOX_SITE_H_

#include <wx/wx.h>
#include <wx/progdlg.h>
#include <list>
#include "hoxTypes.h"
#include "hoxPlayerMgr.h"
#include "hoxTableMgr.h"
#include "hoxLocalPlayer.h"

/**
 * The Site's Actions that are enabled at a given time.
 */
enum hoxSiteAction
{
	/* NOTE: The numeric values are of the 32-bitmap ones. */

	hoxSITE_ACTION_CONNECT        = ( (unsigned int) 1 ),
	hoxSITE_ACTION_DISCONNECT     = ( (unsigned int) 1 << 1 ),

	hoxSITE_ACTION_LIST           = ( (unsigned int) 1 << 2 ),
	hoxSITE_ACTION_NEW            = ( (unsigned int) 1 << 3 ),
    hoxSITE_ACTION_JOIN           = ( (unsigned int) 1 << 4 ),
	hoxSITE_ACTION_CLOSE          = ( (unsigned int) 1 << 5 )
};

/**
 * The Site.
 */
class hoxSite : public wxObject
{
public:
    hoxSite( hoxSiteType             type, 
             const hoxServerAddress& address );
    virtual ~hoxSite();

    hoxSiteType GetType() const { return m_type; }
    hoxServerAddress GetAddress() const { return m_address; }

    virtual const wxString GetName() const { return "_Unknown_"; }

    virtual hoxResult Connect() = 0;
    virtual hoxResult Close() = 0;   // TODO: Should be renamed to "Disconnect()"

    virtual bool IsConnected() const = 0;

    virtual hoxResult QueryForNetworkTables() = 0;
    virtual hoxResult CreateNewTable() = 0;

	virtual hoxResult OnPlayerJoined(const wxString&   tableId,
		                             const wxString&   playerId,
                                     const int         playerScore,
									 const hoxColor    requestColor) = 0;

	virtual hoxResult JoinNewTable(const hoxNetworkTableInfo& tableInfo) = 0;
    virtual hoxResult JoinExistingTable(const hoxNetworkTableInfo& tableInfo) = 0;
    virtual hoxResult DisplayListOfTables(const hoxNetworkTableInfoList& tableList) = 0;

    virtual void OnResponse_LOGIN( const hoxResponse_APtr& response ) = 0;
	virtual void OnResponse_LOGOUT( const hoxResponse_APtr& response ) = 0;

    hoxResult CloseTable(hoxTable_SPtr pTable);
    
    const hoxTableList& GetTables() const { return m_tableMgr.GetTables(); } 
    
    hoxTable_SPtr FindTable( const wxString& tableId ) const
        { return m_tableMgr.FindTable( tableId ); }

    /**
     * Find the Player by ID first.
     * If not found, then return a NULL pointer.
     */
    hoxPlayer* FindPlayer( const wxString& playerId ) const
        { return m_playerMgr.FindPlayer( playerId ); }

    /**
     * Find the Player by ID first.
     * If not found, then create a new DUMMY player with the given score.
     */
    hoxPlayer* GetPlayerById( const wxString& sPlayerId,
                              const int       nScore );

    hoxPlayer* CreateDummyPlayer( const wxString& playerId,
		                          int             score = 1500 )
        { return m_playerMgr.CreateDummyPlayer( playerId, score ); }

	virtual void Handle_ShutdownReadyFromPlayer() = 0;

    virtual hoxLocalPlayer* CreateLocalPlayer(const wxString& playerName) = 0;

	virtual unsigned int GetCurrentActionFlags() const = 0;

protected:
    virtual unsigned int GetBoardFeatureFlags() const;

    void ShowProgressDialog( bool bShow = true );

protected:
    const hoxSiteType  m_type;

    hoxServerAddress   m_address;
    hoxPlayerMgr       m_playerMgr;
    hoxTableMgr        m_tableMgr;

    wxProgressDialog*  m_dlgProgress;

	bool               m_siteClosing;    // The Site is being closed?

    hoxLocalPlayer*    m_player;
            /* The player that this Host uses to connect to the server. */
};

typedef std::list<hoxSite*>  hoxSiteList;

/**
 * The REMOTE Site.
 */
class hoxRemoteSite : public hoxSite
{
public:
    hoxRemoteSite(const hoxServerAddress& address,
                  hoxSiteType             type = hoxSITE_TYPE_REMOTE);
    virtual ~hoxRemoteSite();

    virtual const wxString GetName() const;

    virtual hoxResult Connect();
    virtual hoxResult Close();

    virtual bool IsConnected() const;

    virtual hoxResult QueryForNetworkTables();
    virtual hoxResult CreateNewTable();

	virtual hoxResult OnPlayerJoined(const wxString&   tableId,
		                             const wxString&   playerId,
                                     const int         playerScore,
									 const hoxColor    requestColor);

	virtual hoxResult JoinNewTable(const hoxNetworkTableInfo& tableInfo);
    virtual hoxResult JoinExistingTable(const hoxNetworkTableInfo& tableInfo);
    virtual hoxResult DisplayListOfTables(const hoxNetworkTableInfoList& tableList);

	virtual void Handle_ShutdownReadyFromPlayer();

    virtual hoxLocalPlayer* CreateLocalPlayer(const wxString& playerName);

	virtual unsigned int GetCurrentActionFlags() const;

    virtual void OnResponse_LOGIN( const hoxResponse_APtr& response );
	virtual void OnResponse_LOGOUT( const hoxResponse_APtr& response );

private:
    hoxTable_SPtr _CreateNewTableWithGUI(const hoxNetworkTableInfo& tableInfo);
};

/**
 * The Chesscape (remote) Site.
 */
class hoxChesscapeSite : public hoxRemoteSite
{
public:
    hoxChesscapeSite(const hoxServerAddress& address);
    virtual ~hoxChesscapeSite();

    virtual hoxLocalPlayer* CreateLocalPlayer(const wxString& playerName);

	virtual unsigned int GetCurrentActionFlags() const;

    virtual void OnResponse_LOGIN( const hoxResponse_APtr& response );

protected:
    virtual unsigned int GetBoardFeatureFlags() const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * The Site-Manager.
 * This is implemented as a singleton since we only need one instance.
 */
class hoxSiteManager
{
public:
	static hoxSiteManager* GetInstance();
    ~hoxSiteManager();

	hoxSite* CreateSite( hoxSiteType             siteType, 
		                 const hoxServerAddress& address,
				         const wxString&         userName,
						 const wxString&         password );

	hoxSite* FindSite( const hoxServerAddress& address ) const;
	
	int GetNumberOfSites() const { return (int) m_sites.size(); }

	void DeleteSite( hoxSite* site );

	void Close();

	const hoxSiteList& GetSites() const { return m_sites; }

private:
    hoxSiteManager();
	static hoxSiteManager* m_instance;

private:
	hoxSiteList     m_sites;
};

#endif /* __INCLUDED_HOX_SITE_H_ */
