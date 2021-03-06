

#include "FeaturesDetectionApplication.hpp"
#include <boost/scoped_ptr.hpp>


int main(int argc, char *argv[])
{
    using uniclop::AbstractApplication;
    using uniclop::FeaturesDetectionApplication;
    boost::scoped_ptr<AbstractApplication> application_p(new FeaturesDetectionApplication());
    return application_p->main(argc, argv);
}
