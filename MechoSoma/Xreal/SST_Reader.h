#ifndef __SST_READER__
#define __SST_READER__

#include <list>

struct UsualRecord
{
	int x, y;
	void read(XBuffer& b) { b >= x >= y; }
};

struct DirectedRecord : UsualRecord
{
	int angle;
	void read(XBuffer& b) { UsualRecord::read(b); b >= angle; }
};

int vmapOpenResource(const char* fname,XStream& fh);

template <class Record>
class SST_Reader : public std::list<Record>
{
public:

SST_Reader(const char* fname)
{
	XStream ff(0);
	//if(!ff.open(fname, XS_IN))
	if(!vmapOpenResource(fname, ff))
		return;
	int len = ff.size();
	XBuffer buffer(len + 1);
	ff.read(buffer.address(), len);
	buffer[len] = 0;
	ff.close();
	buffer.search("}");
	++buffer;
	for(;;){
		for(;;){
			if(buffer.end())
				ErrH.Abort("Incorrect SST reading", XERR_USER, buffer.tell() - buffer.length(), fname);
			if(!buffer())
				return;
			if(!isspace(buffer()))
				break;
			++buffer;
			}
		this->push_back(Record());
		this->back().read(buffer);
		}
}
};
		
#endif  // __SST_READER__
