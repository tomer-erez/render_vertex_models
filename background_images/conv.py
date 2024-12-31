import cv2
import os

def convert_bgr_to_rgb(input_path, output_path):
    """
    Convert an image from BGR to RGB and save it to a new location.
    
    Parameters:
    - input_path: str, path to the input image.
    - output_path: str, path to save the converted image.
    """
    # Ensure the output directory exists
    print("*(Q^$%)")
    print(output_path)
    
    # Read the image in BGR format
    image_bgr = cv2.imread(input_path)
    if image_bgr is None:
        print(f"Error: Cannot read the image at {input_path}")
        return
    
    # Convert BGR to RGB
    image_rgb = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2RGB)
    
    # Save the RGB image to the output path
    cv2.imwrite(output_path, image_rgb)
    print(f"Image saved successfully to {output_path}")

# Example usage
if __name__ == "__main__":
    input_image_path = "dog.png"  # Replace with your input image path
    output_image_path = "dog_swapped.png"  # Replace with your output image path
    
    convert_bgr_to_rgb(input_image_path, output_image_path)
