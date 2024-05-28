from PIL import Image
import os

def convert_to_grayscale(image):
    grayscale_image = image.convert("L")
    return grayscale_image

def convert_jpg_to_bmp(jpg_filename, bmp_filename, mode='color'):
    # Mở tệp JPG
    img = Image.open(jpg_filename)
    
    # Chuyển đổi ảnh sang xám nếu chế độ là 'gray'
    if mode == 'gray':
        img = convert_to_grayscale(img)
    
    # Lưu ảnh dưới định dạng BMP
    img.save(bmp_filename, format='BMP')

def check_bmp_info(bmp_filename):
    img = Image.open(bmp_filename)
    print(f'Mode: {img.mode}')
    print(f'Size: {img.size}')
    print(f'Format: {img.format}')

# Sử dụng đúng đường dẫn đến tệp BMP đã tạo
def main():
    jpg_filename = 'raw_img/input1.jpg'
    bmp_filename = 'img/output1.bmp'

    option = input("Chọn chế độ chuyển đổi (1: màu, 2: xám): ")
    if option == '1':
        convert_jpg_to_bmp(jpg_filename, bmp_filename, mode='color')
    elif option == '2':
        convert_jpg_to_bmp(jpg_filename, bmp_filename, mode='gray')
    else:
        print("Chế độ không hợp lệ. Vui lòng chọn 1 hoặc 2.")
        return

    check_bmp_info(bmp_filename)

if __name__ == "__main__":
    main()
