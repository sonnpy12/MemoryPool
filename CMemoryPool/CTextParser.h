#pragma once
class CTextParser
{
public:
	bool    LoadFile();
	void	TextParser();
	bool	FindServer();
	bool    GetServerId();
	bool    GetServerBindIp();
	bool    GetServerBindPort();
	bool    GetWorkerThread();
	bool    GetMaxUser();
private:
	char*   _buf;
	WCHAR   _serverName[64];
	WCHAR	_serverId;
	WCHAR	_server;
};

