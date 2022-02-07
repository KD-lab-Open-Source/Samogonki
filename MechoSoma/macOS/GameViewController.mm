#import "GameViewController.h"

#import <Carbon/Carbon.h>
#import <MetalKit/MetalKit.h>

#include "KeyboardCodeConversion.h"
#include "Renderer.h"

#include "KEYS.H"
#include "xgraph.h"
#include "xtool.h"

@interface GameViewController() <MTKViewDelegate>

@property (nonatomic, retain) MTKView *metalView;
@property (nonatomic, retain) id<MTLLibrary> library;
#ifdef METAL_FRAME_CAPTURE
@property (nonatomic, retain) MTLCaptureDescriptor *captureDescriptor;
@property (nonatomic) BOOL canCapture;
#endif

@end

@implementation GameViewController
{
	int _id;
	int _previous_id;
	XRuntimeObject *_current_rto;
}

- (void)loadView
{
	self.metalView = [MTKView new];
	self.view = self.metalView;
}

- (void)viewDidLoad
{
	[super viewDidLoad];

	self.metalView.device = MTLCreateSystemDefaultDevice();
	self.metalView.delegate = self;
	self.metalView.clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
	self.metalView.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
	self.metalView.frame = NSMakeRect(0, 0, 800, 600);
	self.metalView.preferredFramesPerSecond = 30;

	self.library = [self.metalView.device newDefaultLibrary];
	if (self.library == nil)
	{
		return;
	}

	// create a tracking area to keep track of the mouse movements and events
	NSTrackingAreaOptions options = (NSTrackingActiveAlways 
		| NSTrackingInVisibleRect 
		| NSTrackingMouseEnteredAndExited 
		| NSTrackingMouseMoved);
	NSTrackingArea *area = [[NSTrackingArea alloc] initWithRect:self.view.bounds
		options:options
		owner:self.view
		userInfo:nil];
	[self.view addTrackingArea:area];

#ifdef METAL_FRAME_CAPTURE
    MTLCaptureManager *captureManager = MTLCaptureManager.sharedCaptureManager;
    if ([captureManager supportsDestination:MTLCaptureDestinationGPUTraceDocument])
    {
    	NSLog(@"DEBUG: GPU trace document");
		self.captureDescriptor = [MTLCaptureDescriptor new];
		self.captureDescriptor.destination = MTLCaptureDestinationGPUTraceDocument;
		self.captureDescriptor.captureObject = self.metalView.device;

		self.canCapture = NO;
    }
#endif
	graphics::Renderer::shared = std::make_unique<graphics::Renderer>(
		self.metalView.device,
		self.library
	);

	_previous_id = 0;
	_id = xtInitApplication();
	_current_rto = xtGetRuntimeObject(_id);
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void)keyDown:(NSEvent *)event
{
	const auto key = platform::KeyboardCodeFromNSEvent(event);
	XKey.PressFnc(key, 1);
	KeyBuf->put(VK_SPACE, KBD_CUR_KEY_PRESSED);

#ifdef METAL_FRAME_CAPTURE
	if (key == VK_LEFT)
	{
		self.canCapture = YES;

		NSURL *outputURL = [[NSURL alloc] initFileURLWithPath:NSTemporaryDirectory()];
		const auto time = (long)([NSDate date].timeIntervalSince1970);
		NSString *name = [NSString stringWithFormat:@"Trace_%ld.gputrace", time];
		self.captureDescriptor.outputURL = [outputURL URLByAppendingPathComponent:name];
	}
#endif

	[super keyDown:event];
}

- (void)keyUp:(NSEvent *)event
{
	const auto key = platform::KeyboardCodeFromNSEvent(event);
	XKey.UnPressFnc(key, 1);
	KeyBuf->put(key, KBD_CUR_KEY_UNPRESSED);

	[super keyUp:event];
}

- (void)mouseDown:(NSEvent *)event
{
	[self handleMouseEvent:event];
	[super mouseDown:event];
}

- (void)mouseUp:(NSEvent *)event
{
	[self handleMouseEvent:event];
	[super mouseUp:event];
}

- (void)mouseMoved:(NSEvent *)event
{
	[self handleMouseEvent:event];
	[super mouseMoved:event];
}

- (void)mouseEntered:(NSEvent *)event
{
	[NSCursor hide];
}

- (void)mouseExited:(NSEvent *)event
{
	[NSCursor unhide];
}

- (void)handleMouseEvent:(NSEvent *)event
{
	const auto x = static_cast<int>(event.locationInWindow.x);
	const auto y = static_cast<int>(self.metalView.frame.size.height - event.locationInWindow.y);

	switch (event.type)
	{
		case NSEventTypeMouseMoved:
		{
			const int x1 = XGR_MouseObj.PosX;
			const int y1 = XGR_MouseObj.PosY;

			XGR_MouseObj.InitPos(x, y);

			XGR_MouseObj.MovementX = x - x1;
			XGR_MouseObj.MovementY = y - y1;

			XGR_MouseObj.Move(0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
			if (XGR_MouseVisible())
			{
				XGR_MouseRedraw();
			}
			break;
		}

		case NSEventTypeLeftMouseDown:
		{
			XGR_MouseInitPos(x, y);
			XGR_MousePress(XGM_LEFT_BUTTON, 0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
			break;
		}

		case NSEventTypeRightMouseDown:
		{
			XGR_MouseInitPos(x, y);
			XGR_MousePress(XGM_RIGHT_BUTTON, 0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
			break;
		}

		case NSEventTypeLeftMouseUp:
		{
			XGR_MouseInitPos(x, y);
			XGR_MouseUnPress(XGM_LEFT_BUTTON, 0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
			break;
		}

		case NSEventTypeRightMouseUp:
		{
			XGR_MouseInitPos(x, y);
			XGR_MouseUnPress(XGM_RIGHT_BUTTON, 0, XGR_MouseObj.PosX, XGR_MouseObj.PosY);
			break;
		}

		default:
			break;
	}
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
}

- (void)drawInMTKView:(MTKView *)view
{
	[self startCapture];

	graphics::Renderer::shared->begin(view);

	if (_id)
	{
		_current_rto->Init(_previous_id);
		_previous_id = _id;
		_id = 0;
	}
	else
	{
		_id = _current_rto->Quant();
		if (_id)
		{
			_current_rto->Finit();
			_current_rto = xtGetRuntimeObject(_id);
		}
	}

	graphics::Renderer::shared->end();

	[self stopCapture];
}

- (void)startCapture
{
#ifdef METAL_FRAME_CAPTURE
	if (!self.canCapture)
	{
		return;
	}

	NSError *error = nil;
	BOOL success = [MTLCaptureManager.sharedCaptureManager startCaptureWithDescriptor:self.captureDescriptor error:&error];
	if (!success)
	{
		NSLog(@"ERROR: %@", error);
	}
#endif
}

- (void)stopCapture
{
#ifdef METAL_FRAME_CAPTURE
	if (!self.canCapture)
	{
		return;
	}
	self.canCapture = NO;

	[MTLCaptureManager.sharedCaptureManager stopCapture];
#endif
}

@end
