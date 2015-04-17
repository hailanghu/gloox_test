#include "client.h"
#include "messagesessionhandler.h"
#include "messageeventhandler.h"
#include "messageeventfilter.h"
#include "chatstatehandler.h"
#include "chatstatefilter.h"
#include "connectionlistener.h"
#include "disco.h"
#include "message.h"
#include "gloox.h"
#include "lastactivity.h"
#include "loghandler.h"
#include "logsink.h"
#include "connectiontcpclient.h"
#include "connectionsocks5proxy.h"
#include "connectionhttpproxy.h"
#include "messagehandler.h"
#include "vcard.h"
#include "iq.h"
#include "vcardhandler.h"
#include "vcardmanager.h"
using namespace gloox;

#ifndef _WIN32
# include <unistd.h>
#endif

#include <stdio.h>
#include <string>

#include <cstdio> // [s]print[f]

#if defined(WIN32) || defined(_WIN32)
# include <windows.h>
#endif


class XmppClient : public MessageSessionHandler, ConnectionListener, LogHandler,
	MessageEventHandler, MessageHandler, ChatStateHandler, VCardHandler, IqHandler
{
	std::string jid_, password_, stanza_, nickname_;
	VCardManager *vcard_mgr_;
public:
	XmppClient(std::string const& jid, std::string const& password) : jid_(jid), password_(password), session_(0), messageEventFilter_(0), chatStateFilter_(0) {}
	void set_stanza(std::string const& stanza){stanza_ = stanza;}
	void set_nickname(std::string const& nickname){nickname_ = nickname;}

	virtual ~XmppClient() {}//{vcard_mgr_->cancelVCardOperations(this); delete vcard_mgr_;}

	void start();


	virtual void onConnect();
	

	virtual void onDisconnect(ConnectionError e);
	

	virtual bool onTLSConnect(const CertInfo& info);
	

	virtual void handleMessage(const Message& msg, MessageSession * /*session*/);
	

	virtual void handleMessageEvent(const JID& from, MessageEventType event);
	

	virtual void handleChatState(const JID& from, ChatStateType state);
	

	virtual void handleMessageSession(MessageSession *session);
	

	virtual void handleLog(LogLevel level, LogArea area, const std::string& message);
	

	virtual void handleVCard( const JID& jid, const VCard *v )
	{
	}

	virtual void handleVCardResult( VCardContext context, const JID& jid,
		StanzaError se = StanzaErrorUndefined  )
	{
		j->disconnect();
	}
private:
	Client *j;
	MessageSession *session_;
	MessageEventFilter *messageEventFilter_;
	ChatStateFilter *chatStateFilter_;

public:
	bool handleIq( const IQ& iq );
	

	void handleIqID( const IQ& iq, int context );
	
};