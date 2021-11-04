package motor.panel.project;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class MotorPanel {
	
	private static MotorRequests requests = new MotorRequests();

	public static void main(String[] args) {
		/// Creating the Frame
		JFrame frame = new JFrame("Motor Control");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(300, 125);
		
		// Creating the panel for IP setting
        JPanel panel_ip = new JPanel();
        JLabel label = new JLabel("http://");
        JTextField textfield = new JTextField(15);
        JButton set = new JButton("Set");
        
        // Add button listeners
        set.addActionListener(new ActionListener() {
        	public void actionPerformed(ActionEvent e) {
        		try {
					requests.reqLeft();
				} catch (IOException e1) {
					e1.printStackTrace();
				}
        	} 
        });
        
        panel_ip.add(label);
        panel_ip.add(textfield);
        panel_ip.add(set);
        
		// Creating the panel for Motor Control
        JPanel panel_motor = new JPanel();
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
        
        panel_motor.add(left);
        panel_motor.add(right);
        panel_motor.add(stop);
		
		//Adding Components to the frame.
        frame.getContentPane().add(BorderLayout.NORTH, panel_ip);
        frame.getContentPane().add(BorderLayout.SOUTH, panel_motor);
        frame.setVisible(true);
	}

}
