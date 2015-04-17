#include "xmppclient.h"
#include <string>
#include <iostream>

using namespace std;

int main(int ac, char** av)
{


	string jid = "10820@server.camwowo.com";
	string password = "81f26345e5804522a7c7";
	string nickname = "nick";




	cout << "Login with " << jid << endl;

	XmppClient *r = new XmppClient(jid, password);
	r->set_nickname(nickname);
	r->start();
	delete( r );
	return 0;
}