#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

std::vector<std::string> list_files(const std::string &path, const std::string &ext = "") {
	boost::filesystem::path input_path(path);
	std::vector<std::string> file_list;
	if(boost::filesystem::exists(input_path) && boost::filesystem::is_directory(input_path)) {
		boost::filesystem::directory_iterator end;
		for(boost::filesystem::directory_iterator it(input_path); it != end; ++it) {
			if(!boost::filesystem::is_directory(*it)) {
				if( (ext.length() > 0 && boost::filesystem::extension(*it) == boost::filesystem::path(ext)) ||
					 ext.length() == 0) {
					file_list.push_back(it->path().string());
				}
			}
		}
	}
	return file_list;
}

int main(int argc, char *argv[]) {
	std::string iconfolder_location;
	int icons_x = 30;
	std::string outputimage_location("wp.png");

	if (argc < 2) {
		std::cerr << "Expected at least one input." << std::endl;
		std::cerr << "Format is <icon directory> [icons_x] [output image]" << std::endl;
		exit(1);
	} 

	iconfolder_location = argv[1];

	if (argc > 2) {
		icons_x = atoi(argv[2]);
	}

	if (argc > 3) {
		outputimage_location = argv[4];
	}

	if(!boost::filesystem::exists(iconfolder_location)) {
		std::cerr << "Icon folder " << iconfolder_location << " not found." << std::endl;
		exit(1);
	}

	std::vector<std::string> icon_file_list = list_files(iconfolder_location, ".png");
	std::random_shuffle(icon_file_list.begin(), icon_file_list.end());
	
	if(icon_file_list.size() == 0) {
		std::cerr << "No png images found." << std::endl;
	}

	std::vector<cv::Mat> icon_images(icon_file_list.size());
	std::vector< std::pair<float, int> > hues(icon_images.size());
	for(size_t i=0; i<icon_file_list.size(); i++) {
		icon_images[i] = cv::imread(icon_file_list[i]);
		cv::Mat tmp;
		cv::cvtColor(icon_images[i], tmp, cv::COLOR_RGB2HSV);
		std::vector<cv::Mat> channels(3);
		cv::split(tmp, channels);
		std::vector<int> hist(180, 0);
		for(int j=0; j<tmp.size().area(); j++) {			
			cv::Vec3b hsv_pixel = tmp.at<cv::Vec3b>(j);
			if((int)(hsv_pixel[1]) > 64 && (int)(hsv_pixel[2]) > 32)
				hist[(int)(hsv_pixel[0])]++;			
		}
		auto off = std::max_element(hist.begin(), hist.end());
		hues[i] = std::pair<float, int>(( std::distance(hist.begin(), off) + 70) % 180, i);
	}
	std::sort(hues.begin(), hues.end());


	// assuming all images are same size
	cv::Size iconsize = icon_images[0].size();

	int icons_y = icon_file_list.size() / icons_x;

	std::cout << "Using " << icons_x * icons_y << " of " << icon_file_list.size() << " icons." << std::endl;
	
	cv::Mat wp_image(icons_y * iconsize.height, icons_x * iconsize.width, CV_8UC3);
	for(int y=0, i=0; (y+1) * icons_x < icon_file_list.size(); y++) {
		for(int x=0; x < icons_x; x++, i++) {
			cv::Mat subimage = wp_image(cv::Rect(
				x * iconsize.width, y * iconsize.height,
				iconsize.width, iconsize.height
				));
			subimage.setTo(255);
			cv::resize(icon_images[hues[i].second], subimage, subimage.size(), cv::INTER_AREA);
			
		}
	}

	cv::imwrite(outputimage_location, wp_image);

	return 0;
}