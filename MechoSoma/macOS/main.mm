#import <Cocoa/Cocoa.h>

#import "GameViewController.h"

@interface AppDelegate: NSObject <NSApplicationDelegate, NSWindowDelegate>

@property (nonatomic, retain) NSWindow *window;

@end

@implementation AppDelegate: NSObject

- (id)init
{
    if (self = [super init])
    {
        self.window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 1024, 768) 
                                                  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                                                    backing:NSBackingStoreBuffered
                                                      defer:NO];
        self.window.contentViewController = [[GameViewController alloc] initWithNibName:nil bundle:nil];
        [self.window makeFirstResponder:self.window.contentViewController];
    }
    return self;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    self.window.title = NSProcessInfo.processInfo.processName;
    [self.window makeKeyAndOrderFront:self];
}

@end

int main(int argc, const char * argv[])
{
    @autoreleasepool {
        NSApplication *application = [NSApplication sharedApplication];
        AppDelegate *appDelegate = [AppDelegate new];
        application.delegate = appDelegate;
        [application run];
    }
    return EXIT_SUCCESS;
}
