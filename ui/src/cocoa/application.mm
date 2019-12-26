#import <internal/cocoa/global.h>

#import <internal/cocoa/XamlApplicationDelegate.h>
#include <xaml/ui/application.hpp>

@implementation XamlApplicationDelegate : XamlDelegate

- (id)init
{
    if (self = [super init])
    {
        // allocate and initialize window and stuff here ..
    }
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    [NSApp activateIgnoringOtherApps:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
    return YES;
}

- (void)dealloc
{
    [super dealloc];
}

@end

using namespace std;

namespace xaml
{
    void application::init_components()
    {
        [NSAutoreleasePool new];
        [NSApplication sharedApplication];

        XamlApplicationDelegate* appDelegate = [[[XamlApplicationDelegate alloc] initWithClassPointer:this] autorelease];
        [[NSApplication sharedApplication] setDelegate:appDelegate];
    }

    int application::run()
    {
        [NSApp run];
        return 0;
    }

    application::~application()
    {
        [NSApp terminate:nil];
    }
}
