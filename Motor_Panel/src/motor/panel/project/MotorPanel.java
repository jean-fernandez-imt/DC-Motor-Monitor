package motor.panel.project;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class MotorPanel {
	
	private static MotorRequests requests = new MotorRequests();

	public static void main(String[] args) {
		/// Creating the Frame
		JFrame frame = new JFrame("Motor Control");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(300, 75);
		
		// Creating the panel for Motor Control
        JPanel panel = new JPanel();
        JButton left = new JButton("Left");
        JButton right = new JButton("Right");
        JButton stop = new JButton("Stop");
        
        // Add button listeners
        left.addActionListener(new ActionListener() {
        	public void actionPerformed(ActionEvent e) {
        		try {
					requests.reqLeft();
				} catch (IOException e1) {
					e1.printStackTrace();
				}
        	} 
        });
        
        right.addActionListener(new ActionListener() {
        	public void actionPerformed(ActionEvent e) {
        		try {
					requests.reqRight();
				} catch (IOException e1) {
					e1.printStackTrace();
				}
        	} 
        });
        
        stop.addActionListener(new ActionListener() {
        	public void actionPerformed(ActionEvent e) {
        		try {
					requests.reqStop();
				} catch (IOException e1) {
					e1.printStackTrace();
				}
        	} 
        });
        
        panel.add(left);
        panel.add(right);
        panel.add(stop);
		
		//Adding Components to the frame.
        frame.getContentPane().add(BorderLayout.CENTER, panel);
        frame.setVisible(true);
	}

}
