#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

class File
{
public:
	File( const char *filename );
	~File();

	int size() const;
	const char* data() const;

protected:
	int mSize;
	char* mData;
};

#endif
