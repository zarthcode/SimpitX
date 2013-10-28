// FrameSource.h


// image data source for simpitx consumers.
class FrameDataSource
{
public:

	/// Private, to ensure that only a factory can create this class
	FrameDataSource();

	/// Virtual destructor
	virtual ~FrameDataSource();

	/// Locks and returns the next available frame
	void getFrame(unsigned int frameid, void* target);

	/// Returns the id number of the given frame/data
	unsigned int getFrameId(std::string name);

protected:

private:
	


	
};
