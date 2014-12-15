import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class ImageComparison {
	
	public static void main(String[] args) {
		File imgFile, imgFile2;
		String imgFolder = "../images";
		BufferedImage imgReal, imgTest;
		
		if(args.length != 2) {
			System.out.println("Usage: java ImageReader <filename1> <filename2>");
			System.exit(0);
		}

		imgFile = new File(imgFolder + "/" + args[0]);
		//imgFile2 = new File(imgFolder + "/f1.bmp");
		imgFile2 = new File(imgFolder + "/" + args[1]);

		if(imgFile.exists() && imgFile2.exists()) {
			try {
				imgReal = ImageIO.read(imgFile);
				imgTest = ImageIO.read(imgFile2);
				
				parseImages(imgReal, imgTest);
			}
			catch(IOException e) {}
		}
		else {
			System.out.println("Image file not found.\n");
		}
	}
	
	public static void parseImages(BufferedImage imgReal, BufferedImage imgTest) {
		int height, width;
		int rgb1, r1, g1, b1;
		int rgb2, r2, g2, b2;
		
		if(imgTest.getHeight() != imgReal.getHeight() || imgTest.getWidth() != imgTest.getWidth()) {
			System.out.println("The images do not have the same dimensions.");
			System.exit(0);
		}
		
		height = imgTest.getHeight();
		width = imgTest.getWidth();
		
		float percentage;
		int totalPixels = height * width;
		int errorPixels = 0;
		
		for(int j = 0; j < height; j++) {
			for(int i = 0; i < width; i++) {
				rgb1 = imgReal.getRGB(i, j);
				r1 = (rgb1 >> 16) & 0xFF;
				g1 = (rgb1 >> 8) & 0xFF;
				b1 = (rgb1 >> 0) & 0xFF;
				
				rgb2 = imgTest.getRGB(i, j);
				r2 = (rgb2 >> 16) & 0xFF;
				g2 = (rgb2 >> 8) & 0xFF;
				b2 = (rgb2 >> 0) & 0xFF;
				
				if(r1 != r2 || g1 != g2 || b1 != b2) {
					errorPixels++;
				}
			}
		}
		
		System.out.println("Test completed:");
		System.out.println("Number of pixels wrong: " + errorPixels);
		System.out.println("Error %: " + (double)errorPixels / (double)totalPixels * 100 + " %");
	}
}