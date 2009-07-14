#include "conn.h"

int main(int argc,char* argv[])
{
	Conn m_conn;
	
//	m_conn.set_timeout(7);
	if(m_conn.open_conn("192.168.252.170",80))
	{
		char *buf="hi, i just want to ...";
		int len = strlen(buf);

		if(m_conn.send_data(buf,len))
		{
			cout<<buf<<endl;
			cout<<len<<endl;

			char buffer[1024];
			m_conn.set_timeout(7);
			if(m_conn.recv_data(buffer,1024)>0)
			{
				cout<<buffer<<endl;
			}
		}
	}
	
	return true;
}
