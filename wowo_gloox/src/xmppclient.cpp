#include "xmppclient.h"

void XmppClient::start()
{

	JID jid(jid_);
	j = new Client(jid, password_, 5555);
	j->disableRoster();
	j->registerConnectionListener(this);
	j->registerMessageSessionHandler(this, 0);

	j->registerIqHandler(this, ExtPing);

	j->disco()->setVersion("XmppClient", GLOOX_VERSION, "Linux");
	j->disco()->setIdentity("client", "bot");
	j->disco()->addFeature(XMLNS_CHAT_STATES);
	StringList ca;
	ca.push_back("/path/to/cacert.crt");
	j->setCACerts(ca);

	j->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);
	vcard_mgr_ = new VCardManager(j);

	if(j->connect(false))
	{
		ConnectionError ce = ConnNoError;
		while(ce == ConnNoError)
		{
			ce = j->recv();
		}
		//        printf("ce: %d\n", ce);
	}

	delete(j);
}

void XmppClient::onConnect()
{
	printf("connected!!!\n");

	if (! nickname_.empty())
	{
		VCard *v = new VCard();
		v->setNickname(nickname_);
		vcard_mgr_->storeVCard(v, this);    
	}
}

void XmppClient::onDisconnect( ConnectionError e )
{
	printf("xmppc: disconnected: %d\n", e);

	if(e == ConnAuthenticationFailed)
		printf("auth failed. reason: %d\n", j->authError());
}

bool XmppClient::onTLSConnect( const CertInfo& info )
{
	time_t from(info.date_from);
	time_t to(info.date_to);

	printf("status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
		"from: %s\nto: %s\n",
		info.status, info.issuer.c_str(), info.server.c_str(),
		info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
		info.compression.c_str(), ctime(&from), ctime(&to));
	return true;
}

void XmppClient::handleMessage( const Message& msg, MessageSession * /*session*/ )
{
	printf("type: %d, subject: %s, message: %s, thread id: %s\n", msg.subtype(),
		msg.subject().c_str(), msg.body().c_str(), msg.thread().c_str());

	std::string re = "You said:\n> " + msg.body() + "\nI like that statement.";
	std::string sub;
	if(!msg.subject().empty())
		sub = "Re: " +  msg.subject();

	messageEventFilter_->raiseMessageEvent(MessageEventDisplayed);
#if defined(WIN32) || defined(_WIN32)
	Sleep(1000);
#else
	sleep(1);
#endif
	messageEventFilter_->raiseMessageEvent(MessageEventComposing);
	chatStateFilter_->setChatState(ChatStateComposing);
#if defined(WIN32) || defined(_WIN32)
	Sleep(2000);
#else
	sleep(2);
#endif
	session_->send(re, sub);

	if(msg.body() == "quit")
		j->disconnect();
}

void XmppClient::handleMessageEvent( const JID& from, MessageEventType event )
{
	printf("received event: %d from: %s\n", event, from.full().c_str());
}

void XmppClient::handleChatState( const JID& from, ChatStateType state )
{
	printf("received state: %d from: %s\n", state, from.full().c_str());
}

void XmppClient::handleMessageSession( MessageSession *session )
{
	printf("got new session\n");
	// this example can handle only one session. so we get rid of the old session
	j->disposeMessageSession(session_);
	session_ = session;
	session_->registerMessageHandler(this);
	messageEventFilter_ = new MessageEventFilter(session_);
	messageEventFilter_->registerMessageEventHandler(this);
	chatStateFilter_ = new ChatStateFilter(session_);
	chatStateFilter_->registerChatStateHandler(this);
}

void XmppClient::handleLog( LogLevel level, LogArea area, const std::string& message )
{
	printf("log: level: %d, area: %d, %s\n", level, area, message.c_str());
}

//---------IqHandler------------------
bool XmppClient::handleIq( const IQ& iq )
{
	printf("handleIq...\n");

	return true;
}

void XmppClient::handleIqID( const IQ& iq, int context )
{
	printf("handleIqID...\n");
}
