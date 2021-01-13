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
    wc.hCursor = LoadCursor(0, IDC_ARROW);

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

void SetRunning(bool value)
{
    running = value;
}

win32BackBuffer_t* CreateBackBuffer(void)
{
    return &backBuffer;
}

void DrawTileMapInt(int columns, int rows, int tiles[], rect_t tileInfo, int scale, texture_t texture, win32BackBuffer_t* backBuffer)
{
    int posX = tileInfo.x;
    int posY = tileInfo.y;
    for(int y = 20; y > 0; y--)
    {
        for(int x = 0; x < 30; x++)
        {
            tileInfo.x = ((tileInfo.width * x) * scale) + posX;
            tileInfo.y = ((tileInfo.height * (20 - y)) * scale) + posY;
            tileInfo.row =  tiles[((y - 1) * 30) + x] % columns;
            tileInfo.column = tiles[((y - 1) * 30) + x] / rows;
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

void LoadMapFromFile(const char* filePath, tileMap_t* map)
{
    FILE* file = fopen(filePath, "r");
    if(file != NULL)
    {
        char line[1024];
        int incBottom = 0;
        int incTop = 0;
        int incColitions = 0;

        while(fgets(line, 1024, file) != NULL)
        {
            if(strncmp(line, "1:", 2) == 0)
            { 
                sscanf(line, "1: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
                &map->bottomLayer[0 + incBottom],    &map->bottomLayer[1 + incBottom],  &map->bottomLayer[2 + incBottom],
                &map->bottomLayer[3 + incBottom],    &map->bottomLayer[4 + incBottom],  &map->bottomLayer[5 + incBottom],
                &map->bottomLayer[6 + incBottom],    &map->bottomLayer[7 + incBottom],  &map->bottomLayer[8 + incBottom],
                &map->bottomLayer[9 + incBottom],    &map->bottomLayer[10 + incBottom], &map->bottomLayer[11 + incBottom],
                &map->bottomLayer[12 + incBottom],   &map->bottomLayer[13 + incBottom], &map->bottomLayer[14 + incBottom],
                &map->bottomLayer[15 + incBottom],   &map->bottomLayer[16 + incBottom], &map->bottomLayer[17 + incBottom],
                &map->bottomLayer[18 + incBottom],   &map->bottomLayer[19 + incBottom], &map->bottomLayer[20 + incBottom],
                &map->bottomLayer[21 + incBottom],   &map->bottomLayer[22 + incBottom], &map->bottomLayer[23 + incBottom],
                &map->bottomLayer[24 + incBottom],   &map->bottomLayer[25 + incBottom], &map->bottomLayer[26 + incBottom],
                &map->bottomLayer[27 + incBottom],   &map->bottomLayer[28 + incBottom], &map->bottomLayer[29 + incBottom]);
                incBottom += 30; 
            }   
            if(strncmp(line, "2:", 2) == 0)
            {
                sscanf(line, "2: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
                &map->topLayer[0 + incTop],  &map->topLayer[1 + incTop],  &map->topLayer[2 + incTop],
                &map->topLayer[3 + incTop],  &map->topLayer[4 + incTop],  &map->topLayer[5 + incTop],
                &map->topLayer[6 + incTop],  &map->topLayer[7 + incTop],  &map->topLayer[8 + incTop],
                &map->topLayer[9 + incTop],  &map->topLayer[10 + incTop], &map->topLayer[11 + incTop],
                &map->topLayer[12 + incTop], &map->topLayer[13 + incTop], &map->topLayer[14 + incTop],
                &map->topLayer[15 + incTop], &map->topLayer[16 + incTop], &map->topLayer[17 + incTop],
                &map->topLayer[18 + incTop], &map->topLayer[19 + incTop], &map->topLayer[20 + incTop],
                &map->topLayer[21 + incTop], &map->topLayer[22 + incTop], &map->topLayer[23 + incTop],
                &map->topLayer[24 + incTop], &map->topLayer[25 + incTop], &map->topLayer[26 + incTop],
                &map->topLayer[27 + incTop], &map->topLayer[28 + incTop], &map->topLayer[29 + incTop]);
                incTop += 30;           
            }
            if(strncmp(line, "3:", 2) == 0)
            {
                sscanf(line, "3: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
                &map->colitions[0 + incColitions],  &map->colitions[1 + incColitions],  &map->colitions[2 + incColitions],
                &map->colitions[3 + incColitions],  &map->colitions[4 + incColitions],  &map->colitions[5 + incColitions],
                &map->colitions[6 + incColitions],  &map->colitions[7 + incColitions],  &map->colitions[8 + incColitions],
                &map->colitions[9 + incColitions],  &map->colitions[10 + incColitions], &map->colitions[11 + incColitions],
                &map->colitions[12 + incColitions], &map->colitions[13 + incColitions], &map->colitions[14 + incColitions],
                &map->colitions[15 + incColitions], &map->colitions[16 + incColitions], &map->colitions[17 + incColitions],
                &map->colitions[18 + incColitions], &map->colitions[19 + incColitions], &map->colitions[20 + incColitions],
                &map->colitions[21 + incColitions], &map->colitions[22 + incColitions], &map->colitions[23 + incColitions],
                &map->colitions[24 + incColitions], &map->colitions[25 + incColitions], &map->colitions[26 + incColitions],
                &map->colitions[27 + incColitions], &map->colitions[28 + incColitions], &map->colitions[29 + incColitions]);
                incColitions += 30;           
            }        
        }     
    }
    else
    {
        printf("cannot read the file %s\n", filePath);
    }
    fclose(file);
}


void DrawLifeBar(rect_t sprite, int life, win32BackBuffer_t* backBuffer)
{

    int actualWidth = life * sprite.width / 100;
    uint32_t color = 0xFF00FF00;
    if(life <= 50 && life > 25)
        color = 0xFFFFFF00;
    else if(life <= 25)
        color = 0xFFFF0000;

    for(int x = sprite.x; x < sprite.x + sprite.width; x++)
    {
        for(int y = sprite.y; y < sprite.y + sprite.height; y++)
        {
            DrawPixel(x, y, 0xFFDDDDDD, backBuffer);
        }
    }


    for(int x = sprite.x; x < sprite.x + actualWidth; x++)
    {
        for(int y = sprite.y; y < sprite.y + sprite.height; y++)
        {
            DrawPixel(x, y, color, backBuffer);
        }
    }
}

