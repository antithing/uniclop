#include "VideoInputApplication.hpp"

#include "devices/video/ImagesInput.hpp"

// Cimg http://cimg.sf.net
#include <CImg/CImg.h>

#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>
#include <boost/cstdint.hpp>
#include <boost/mpl/assert.hpp>

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cassert>

#include <CImg/CImg.h>
#include "helpers/rgb8_cimg_t.hpp"

namespace uniclop
{

using namespace cimg_library;
using boost::uint8_t;

string VideoInputApplication::get_application_title() const
{

    return "Video input. A simple test application. Uniclop 2009";
}

args::options_description VideoInputApplication::get_command_line_options(void) const
{

    args::options_description desc;

    // desc.add(ImagesInput<uint8_t>::get_options_description());
    desc.add(GstVideoInput::get_options_description());

    return desc;
}


void VideoInputApplication::init_video_input(args::variables_map &options)
{

    gst_video_input_p.reset(new GstVideoInput(options));

    return;
}

int VideoInputApplication::main_loop(args::variables_map &options)
{

    using  boost::gil::copy_pixels;
    using boost::gil::rgb8_planar_image_t;

    printf("VideoInputApplication::main_loop says hello world !\n");

    //init_gui(options);
    //run_gui();

    init_video_input(options);

    // rgb8_cimg_t adapts cimg and gil::image_view
    rgb8_cimg_t current_image(gst_video_input_p->get_image_dimensions());
    gst_video_input_p->get_new_image(current_image.view); // copy the data


    // FIXME should port ImagesInput to Gil
    // ImagesInput<uint8_t> images_input(options);
    // const CImg<uint8_t> &current_image = images_input.get_new_image();

    CImgDisplay video_display(current_image.dimx(), current_image.dimy(), get_application_title().c_str());
    video_display.show();
    video_display.display(current_image);

    do
    {
        gst_video_input_p->get_new_image(current_image.view); // copy the data
        video_display.display(current_image);

        const float seconds_to_wait = 0.1; // [seconds]
        boost::xtime t_time;
        boost::xtime_get(&t_time, boost::TIME_UTC);
        t_time.nsec += (1000 * 1000) * seconds_to_wait;
        boost::thread::sleep(t_time);

    }
    while (video_display.is_closed == false);

    return 0;
}


} // end of namespace uniclop

