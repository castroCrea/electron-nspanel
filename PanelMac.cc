#import <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <objc/objc-runtime.h>

#include <napi.h>

using namespace Napi;

@interface PROPanel : NSPanel
@end

@implementation PROPanel
- (BOOL)needsPanelToBecomeKey
{
  return YES;
}
- (BOOL)acceptsFirstResponder
{
  return YES;
}
@end

Value MakePanel(const CallbackInfo &info)
{
  Env env = info.Env();
  char *handleBuffer = info[0].As<Napi::Buffer<char>>().Data();
  // bool borderless = info[1].As<Napi::Boolean>();

  NSView *mainContentView = *reinterpret_cast<NSView **>(handleBuffer);

  if (!mainContentView)
    return Napi::Boolean::New(env, false);

  object_setClass(mainContentView.window, [PROPanel class]);

  [mainContentView.window setStyleMask:NSBorderlessWindowMask];

  // mainContentView.window.styleMask |= NSWindowStyleMaskNonactivatingPanel;
  [mainContentView.window setCollectionBehavior:NSWindowCollectionBehaviorTransient | NSWindowCollectionBehaviorFullScreenAuxiliary];
  [mainContentView.window setLevel:NSFloatingWindowLevel];

  // [[mainContentView.window standardWindowButton:NSWindowCloseButton] setHidden:YES];
  // [[mainContentView.window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
  // [[mainContentView.window standardWindowButton:NSWindowZoomButton] setHidden:YES];
  // [mainContentView.window setFloatingPanel:YES];

  return Napi::Boolean::New(env, true);
}

Value MakeKeyWindow(const Napi::CallbackInfo &info)
{
  Env env = info.Env();
  char *handleBuffer = info[0].As<Napi::Buffer<char>>().Data();
  NSView *mainContentView = *reinterpret_cast<NSView **>(handleBuffer);

  if (!mainContentView)
    return Napi::Boolean::New(env, false);

  [mainContentView.window makeKeyWindow];
  return Napi::Boolean::New(env, true);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "MakePanel"), Napi::Function::New(env, MakePanel));
  exports.Set(Napi::String::New(env, "MakeKeyWindow"), Napi::Function::New(env, MakeKeyWindow));
  return exports;
}

NODE_API_MODULE(PanelMac, InitAll)
