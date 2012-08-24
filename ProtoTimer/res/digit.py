# -*- coding:utf-8 -*-
import Image, ImageDraw, ImageFont

FONT_PIXEL  = 32

font = ImageFont.truetype('CONSOLA.ttf', FONT_PIXEL)

#这个脚本把数字输出为图像
'''
for i in range(10):
    str = '%d' % i
    im   = Image.new('RGB', (16, 32), '#FFFFFF')
    draw = ImageDraw.Draw(im)
    draw.text((0,0), str, font = font, fill = '#000000')
    im.save('%d.bmp' % i, 'bmp')
'''

im   = Image.new('RGB', (16, 32), '#FFFFFF')
draw = ImageDraw.Draw(im)
draw.text((0,0), '-', font = font, fill = '#000000')
im.save('-.bmp', 'bmp')
