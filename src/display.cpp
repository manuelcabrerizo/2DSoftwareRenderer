#include "display.h"

global_variable win32BackBuffer_t backBuffer;

global_variable bool running;

LRESULT CALLBACK Win32WindowProc(
        HWND Window,
        UINT Message,
        WPARAM wParam,
        LPARAM lParam)
{
    LRESULT result = 0;

    switch(Message)
    {
        case WM_CLOSE:
        {
            running = false;
        }break;

        case WM_DESTROY:
        {
            running = false;
        }break;
        
        case WM_KEYUP:
        {
 			uint32_t  VKCode = wParam;
			bool wasDown = ((lParam & (1 << 30)) != 0);
			bool isDown = ((lParam & (1 << 31)) == 0);
			if (wasDown != isDown)
			{
				SetScanCodeUp(VKCode);
			}  
        }break;

        case WM_KEYDOWN:
        {
 			uint32_t  VKCode = wParam;
			bool wasDown = ((lParam & (1 << 30)) != 0);
			bool isDown = ((lParam & (1 << 31)) == 0);
			if (wasDown != isDown)
			{
				SetScanCodeDown(VKCode);
			} 
        }break;

        default:
        {
            result = DefWindowProc(Window, Message, wParam, lParam);
        } break;
    }

    return (result);
}

HWND Win32CreateWindow(const char* windowName, int width, int height, HINSTANCE hInstance)
{

    running = true;
    // TODO: register the window Class.
    WNDCLASS wc = { };
    wc.lpfnWndProc = Win32WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = windowName;

    // TODO: Create the window.
    HWND hwnd;
    if(RegisterClassA(&wc) != 0)
    {
        RECT rc = {0, 0, width, height};
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
        hwnd = CreateWindowEx(
            0,                              // optional window styles.
            wc.lpszClassName,               // Window CLass.
            windowName,                     // Window Text.
            WS_OVERLAPPEDWINDOW|WS_VISIBLE, // Window Style.
            //Size and position
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rc.right - rc.left,
            rc.bottom - rc.top,
            NULL,                           // Parent Window.
            NULL,                           // Menu.
            hInstance,
            NULL);

         if(hwnd != NULL)
             return hwnd;
         else
             return NULL;
    }
    else
    {
        return NULL;
    }
}

void Win32InputHandler(HWND hwnd)
{
    MSG msg;
    BOOL bRet;

    bRet = PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE);
    if(bRet == -1)
    {
        // TODO: Handle the error and possibly exit
    }
    else
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
}

void Win32SetBackBuffer(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    if(backBuffer->memory)
    {
        VirtualFree(backBuffer->memory, 0, MEM_RELEASE);
    }
    backBuffer->width = width;
    backBuffer->height = height;
    if(backBuffer->handle)
    {
        DeleteObject(backBuffer->handle);
    }   

    backBuffer->info.bmiHeader.biSize = sizeof(backBuffer->info.bmiHeader);
    backBuffer->info.bmiHeader.biWidth = width;
    backBuffer->info.bmiHeader.biHeight = height;
    backBuffer->info.bmiHeader.biPlanes = 1;
    backBuffer->info.bmiHeader.biBitCount = 32;
    backBuffer->info.bmiHeader.biCompression = BI_RGB;

    HDC bitmapDeviceContext = CreateCompatibleDC(0);

    backBuffer->handle = CreateDIBSection(
            bitmapDeviceContext, &backBuffer->info,
            DIB_RGB_COLORS,
            &backBuffer->memory,
            0, 0);
}

void Win32UpdateBackBuffer(HDC deviceContext, int x, int y, int width, int height, win32BackBuffer_t* backBuffer)
{
    HDC hdcMem = CreateCompatibleDC(deviceContext);
    SelectObject (hdcMem, backBuffer->handle) ;
    BitBlt(deviceContext, 0, 0, backBuffer->width, backBuffer->height, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);   
}


void Win32ClearBackBuffer(uint32_t color, win32BackBuffer_t* backBuffer)
{
    uint32_t* buffer = (uint32_t*)backBuffer->memory;

    for(int y = 0; y < backBuffer->height; y++)
    {
        for(int x = 0; x < backBuffer->width; x++)
        {
            buffer[(backBuffer->width * y) + x] = color;
        }
    }
}

void ClearBackBuffer(uint32_t color, win32BackBuffer_t* backBuffer, HWND hwnd)
{
    // Get the Client Rect Size
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    HDC deviceContext = GetDC(hwnd);
    int x = clientRect.left;
    int y = clientRect.top;
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    Win32UpdateBackBuffer(deviceContext, x, y, width, height, backBuffer);
    Win32ClearBackBuffer(color, backBuffer);
    ReleaseDC(hwnd, deviceContext);
}

void DrawPixel(int x, int y, uint32_t color, win32BackBuffer_t* backBuffer)
{
    uint32_t* buffer = (uint32_t*)backBuffer->memory;
    if(x >= 0 && x < backBuffer->width && y >= 0 && y < backBuffer->height)
        buffer[(backBuffer->width * y) + x] = color;
}

void DrawRect(int xPos, int yPos, int width, int height, uint32_t color, win32BackBuffer_t* backBuffer)
{
    for(int x = xPos; x < xPos + width; x++)
    {
        for(int y = yPos; y < yPos + height; y++)
        {
            DrawPixel(x, y, color, backBuffer);
        }
    }
}


void DrawTexture(int xPos, int yPos, texture_t texture, win32BackBuffer_t* backBuffer)
{
    int x1 = 0; 
    for(int x = xPos; x < xPos + texture.width; x++)
    {
        int y1 = 0;
        for(int y = yPos; y < yPos + texture.height; y++)
        { 
            DrawPixel(x, y, (uint32_t)texture.pixels[(y1 * texture.width) + x1], backBuffer); 
            y1++;
        }
        x1++;
    } 
}

void DrawTextureScale(int xPos, int yPos, int scale, texture_t texture, win32BackBuffer_t* backBuffer)
{
    int x1 = 0; 
    for(int x = xPos; x < xPos + texture.width * scale; x += scale)
    {
        int y1 = 0;
        for(int y = yPos; y < yPos + texture.height * scale; y += scale)
        { 
            DrawRect(x, y, scale, scale, (uint32_t)texture.pixels[(y1 * texture.width) + x1], backBuffer);
            y1++;
        }
        x1++;
    } 
}

void DrawFrameTexture(rect_t srcRect, int scale, texture_t texture, win32BackBuffer_t* backBuffer)
{
    int x1 = srcRect.row * srcRect.width; 
    for(int x = srcRect.x; x < srcRect.x + (srcRect.width * scale); x += scale)
    {
        int y1 = srcRect.column * srcRect.height;
        for(int y = srcRect.y; y < srcRect.y + (srcRect.height * scale); y += scale)
        {
            uint32_t pixel = (uint32_t)texture.pixels[(y1 * texture.width) + x1];
            uint8_t a = (uint8_t)(pixel >> 24);   
            if(a > 128)
            {
                DrawRect(x, y, scale, scale, (uint32_t)texture.pixels[(y1 * texture.width) + x1], backBuffer);
            }
            y1++;
        }
        x1++;
    }  
}

texture_t LoadBMP(const char* filename)
{
    bitmapHeader header;
    texture_t texture;
    FILE* file = fopen(filename, "rb");
    if(file != NULL)
    {
        fread(&header, sizeof(header), 1, file);
        texture.pixels = (uint32_t*)malloc(sizeof(uint32_t) * header.width * header.height);
        fseek(file, header.bitmapOffset, SEEK_SET);
        fread(texture.pixels, sizeof(uint32_t), header.width * header.height, file);
        texture.width = header.width;
        texture.height = header.height;
    }
    else
    {
        printf("cannot read the file %s\n", filename);
    }
    fclose(file);
    return(texture);
}

bool GetRunning(void)
{
    return running;
}

win32BackBuffer_t* CreateBackBuffer(void)
{
    return &backBuffer;
}

void DrawTileMapInt(int tiles[], rect_t tileInfo, int scale, texture_t texture, win32BackBuffer_t* backBuffer)
{
    int posX = tileInfo.x;
    int posY = tileInfo.y;
    for(int y = 20; y > 0; y--)
    {
        for(int x = 0; x < 30; x++)
        {
            tileInfo.x = ((tileInfo.width * x) * scale) + posX;
            tileInfo.y = ((tileInfo.height * (20 - y)) * scale) + posY;
            tileInfo.row =  tiles[((y - 1) * 30) + x] % 6;
            tileInfo.column = tiles[((y - 1) * 30) + x] / 6;
            if(tileInfo.row == 0 && tileInfo.column == 0)
            {
                continue;
            }
            DrawFrameTexture(tileInfo, scale, texture, backBuffer);
        }
    }    
}



void DrawString(const char* message, int posX, int posY, texture_t texture, win32BackBuffer_t* backBuffer)
{
    const int length = strlen(message);
    int xIndices[length];
    for(int i = 0; i < length; i++)
    {
        if(message[i] == 32)
            xIndices[i] = 26;
        else
            xIndices[i] = message[i] - 'A'; 
    }
    int index = 0;
    for(int x = 0; x < length * (texture.width / 27); x += (texture.width / 27))
    {
        rect_t srcRect;
        srcRect.x = posX + x;
        srcRect.y = posY;
        srcRect.width = (texture.width / 27);
        srcRect.height = texture.height;
        srcRect.row = xIndices[index];
        srcRect.column = 0;

        DrawFrameTexture(srcRect, 1, texture, backBuffer); 
        index++;
    }

 }

