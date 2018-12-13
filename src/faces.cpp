#include "faces.h"

using namespace cv;
using namespace std;
namespace fs = boost::filesystem;

Faces::Faces()
{
}

int Faces::find(string in, string out)
{
	Mat image;
	// Load the image
	try
	{
		image = imread((in), CV_LOAD_IMAGE_COLOR);
	}
	catch (const std::exception&)
	{
		cout << "Image not found\n";
		return 1;
	}

	// Load the training data
	CascadeClassifier facecascade;
	fs::path data_frontalface = fs::current_path();
	data_frontalface /= "haarcascade_frontalface_alt2.xml";
	facecascade.load("haarcascade_frontalface_alt2.xml");

	// Detect the faces using the training data
	Mat image_gray;
	cvtColor(image, image_gray, COLOR_BGR2GRAY);
	vector<Rect> faces;
	facecascade.detectMultiScale(image_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Draw around the faces
	for (auto const& face : faces)
	{
		Point center(face.x + face.width*0.5, face.y + face.height*0.5);
		ellipse(image, center, Size(face.width*0.5, face.height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}

	// Save the image we draw on to disk
	imwrite(out, image);

	return 0;
}

Faces::~Faces()
{
}