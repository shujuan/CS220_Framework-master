
import java.awt.*;  
import java.awt.image.BufferedImage;  
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

import javax.swing.*;  


public class ShowImage extends JPanel  
{  
	BufferedImage image;  
	Dimension size;  
	String filename ; 

	public ShowImage( String file )  
	{  
		size = new Dimension(200, 200);
		filename = new String(file) ; 
	}  
	
	Color getColor( int index )  {
		
		int rindex = index % 12 ; 
		
		Color[] acolor = new Color[12];  

		acolor[0] = Color.WHITE ; 
		acolor[1] = Color.YELLOW ; 
		acolor[2] = Color.MAGENTA ; 
		acolor[3] = Color.CYAN ;
		acolor[4] = new Color( 0, 255, 127) ; 
		acolor[5] = Color.PINK ;
		acolor[6] = Color.GREEN ; 
		acolor[7] = Color.BLACK ; 
		acolor[8] = Color.ORANGE ;
		acolor[9] = Color.RED ;
		acolor[10] =  Color.BLUE ; 
		acolor[11] = new Color(127, 255, 0); 
		  
		
		return acolor[rindex] ; 
		
		
	}
	
	private void fillRect(BufferedImage bimage, int factor )  {

		Color rcolor = new Color(0,0,0);

		Graphics2D g2 = bimage.createGraphics();  

		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);

		g2.setPaint( rcolor );  

		try {

			for ( int x=0; x < bimage.getWidth(); x = x+factor )  {

				g2.drawLine(x, 0, x, bimage.getHeight() ); ;

			}

			for ( int y=0; y < bimage.getHeight(); y = y+factor )  {

				g2.drawLine(0, y, bimage.getWidth(), y ); ;

			}

			g2.setPaint( Color.RED );  
			g2.drawString(filename, 20, 20); ; 
			

		} catch (Exception ex)  {

		}


	}

	BufferedImage readImage() {

		BufferedImage bufImage = null; 

		BufferedReader br = null; 

		int factor = 20 ; 

		try {

			br = new BufferedReader(new FileReader(filename));

			String line = br.readLine();

			StringTokenizer st = new StringTokenizer(line); 

			int w1 = Integer.parseInt(st.nextToken());
			int h1 = Integer.parseInt(st.nextToken());

			int w = factor * (w1) ; 
			int h = factor * (h1); 

			bufImage = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);  

			Graphics2D g2 = bufImage.createGraphics();  

			int row = 0 ; 

			while (line != null && (row < h1) ) {

				line = br.readLine();

				int col = 0 ;

				st = new StringTokenizer(line, "|" ); 

				while ( st.hasMoreElements() && (col < w1) )  {

					// Processing every single cell 

					String cell = st.nextToken() ; 

					StringTokenizer cellst = new StringTokenizer(cell," ");

					String t1 = cellst.nextToken() ;  // type 
					String t2 = cellst.nextToken() ; // Pos x 
					String t3 = cellst.nextToken() ; // Post y 

					String t5 = "0";   
					
					Color rcolor = new Color(255, 255, 255);

					if ( t1.equals("B") )  {
						
						rcolor = new Color(100, 100, 100);
					}

					else if ( t1.equals("P") )  {

						String t4 = cellst.nextToken() ; // Net ID 
						rcolor = getColor(Integer.parseInt(t4));
						t5 = cellst.nextToken() ; // Priority  
						

					} 

					else if ( t1.equals("C") )  {
						
						String t4 = cellst.nextToken() ; // Net ID 
						rcolor = getColor(Integer.parseInt(t4));
						t5 = cellst.nextToken() ; // Priority 
						
					}
					
					g2.setPaint( rcolor );  

					g2.fillRect(col*factor, row*factor, factor, factor );  
					
					g2.setPaint(Color.BLACK);
					
					g2.drawString(t5, (col*factor+factor/4), (row*factor+factor/2));

					System.out.println( " Filing row " + row + " Col " + col );

					col ++ ; 

				}  // End processing row 

				row ++ ; 

			} // End reading all rows 

			fillRect( bufImage, factor ); 

		} 
		catch ( Exception e)  {

			System.out.println( e.getMessage() );

		} finally {

			try { 

				br.close();

			} catch (Exception e) {} 

		}

		return bufImage ; 

	}


	protected void paintComponent(Graphics g)  {  

		super.paintComponent(g);  

		if(image == null) {  
			image = readImage() ; 
		}

		g.drawImage(image, 0, 0, this);  

	}  

	public Dimension getPreferredSize() { 

		return size;  
	}  

	public static void main(String[] args) 	{

		String fn = "f:\\testcases\\"; 

		ShowImage it1 = new ShowImage(fn+"file037.txt");
		ShowImage it2 = new ShowImage(fn+"file01.txt");
		ShowImage it3 = new ShowImage(fn+"file02.txt");
		ShowImage it4 = new ShowImage(fn+"file03.txt");
		ShowImage it5 = new ShowImage(fn+"file04.txt");
		ShowImage it6 = new ShowImage(fn+"file05.txt");
		ShowImage it7 = new ShowImage(fn+"file06.txt");
		ShowImage it8 = new ShowImage(fn+"file07.txt");
		ShowImage it9 = new ShowImage(fn+"file08.txt");

/*
		ShowImage it1 = new ShowImage(fn+"file07.txt");  
		ShowImage it2 = new ShowImage(fn+"file08.txt");
		ShowImage it3 = new ShowImage(fn+"file09.txt");
		ShowImage it4 = new ShowImage(fn+"file010.txt");
		ShowImage it5 = new ShowImage(fn+"file011.txt");
		ShowImage it6 = new ShowImage(fn+"file012.txt");
		ShowImage it7 = new ShowImage(fn+"file013.txt");
		ShowImage it8 = new ShowImage(fn+"file014.txt");
		ShowImage it9 = new ShowImage(fn+"file015.txt");

		ShowImage it1 = new ShowImage(fn+"file022.txt");  
		ShowImage it2 = new ShowImage(fn+"file023.txt");
		ShowImage it3 = new ShowImage(fn+"file024.txt");
		ShowImage it4 = new ShowImage(fn+"file025.txt");
		ShowImage it5 = new ShowImage(fn+"file026.txt");
		ShowImage it6 = new ShowImage(fn+"file027.txt");
		ShowImage it7 = new ShowImage(fn+"file028.txt");
		ShowImage it8 = new ShowImage(fn+"file029.txt");
		ShowImage it9 = new ShowImage(fn+"file030.txt");


	
		ShowImage it1 = new ShowImage(fn+"file026.txt");  
		ShowImage it2 = new ShowImage(fn+"file027.txt");
		ShowImage it3 = new ShowImage(fn+"file028.txt");
		ShowImage it4 = new ShowImage(fn+"file029.txt");
		ShowImage it5 = new ShowImage(fn+"file030.txt");
		ShowImage it6 = new ShowImage(fn+"file031.txt");
		ShowImage it7 = new ShowImage(fn+"file032.txt");
		ShowImage it8 = new ShowImage(fn+"file033.txt");
		ShowImage it9 = new ShowImage(fn+"file041.txt");
		
	
		ShowImage it1 = new ShowImage(fn+"file042.txt");  
		ShowImage it2 = new ShowImage(fn+"file044.txt");
		ShowImage it3 = new ShowImage(fn+"file045.txt");
		ShowImage it4 = new ShowImage(fn+"file046.txt");
		ShowImage it5 = new ShowImage(fn+"file047.txt");
		ShowImage it6 = new ShowImage(fn+"file048.txt");
		ShowImage it7 = new ShowImage(fn+"file049.txt");
		ShowImage it8 = new ShowImage(fn+"file050.txt");
		ShowImage it9 = new ShowImage(fn+"file051.txt");
*/

		JFrame f = new JFrame();  

		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  

		JPanel panel = new JPanel();
		panel.setAutoscrolls(true);
		panel.setPreferredSize(new Dimension( 1500,1500));
		
		JScrollPane scroller = new JScrollPane(panel);  
		
		scroller.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		scroller.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
	        
		f.getContentPane().add(scroller, BorderLayout.CENTER); 
		
		GridLayout experimentLayout = new GridLayout(1,1);
		
		panel.setLayout( experimentLayout  );

		panel.add(it1 );
	//	panel.add(it2 );
	//	panel.add(it3 );
	//	panel.add(it4 );
	//	panel.add(it5 );
	//	panel.add(it6 );
	//	panel.add(it7 );
	//	panel.add(it8 );
	//	panel.add(it9 );
	//	panel.add(it10 );

		
		f.setSize(800,600);  
		f.setLocation(100,100);
		f.setVisible(true);
		
		System.out.println( "  ");

	}  
}  
