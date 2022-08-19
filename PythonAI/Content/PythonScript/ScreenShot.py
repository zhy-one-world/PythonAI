import pandas as pd
from PIL import Image, ImageDraw
def draw_img(rgbas: list, width: int = 200, height: int = 150):
    """根据所给的RGBA值生成图片，默认宽200高150"""
    # 透明背景，RGBA值为：(0, 0, 0, 0)
    # img = Image.new('RGB', (width, height), (0, 0, 0))
    img = Image.new('RGBA', (width, height), (0, 0, 0, 0))
    rgbas= Image.fromarray(rgbas)
    # 填充像素
    img.putdata(rgbas)
    # 显示图片
    img.show()
    # 保存图片
    img.save('pixel_img/hydrops.png')
print("Load ScreenShot Success")