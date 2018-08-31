# Lab 6

## Assignment

1. There is a sender which wants to communicate with two receivers. How the receivers will know that message is for them? In between the sender and receiver, there is a demultiplexer which receives the messages from the sender and figures out what to send to whom. The sender writes the desired data into a FIFO. 

    The first one is 4 bytes of information which indicates the destination. The second 4 bytes is the length of the message. The next space contains the data that is being sent. The demultiplexer will read from the FIFO and write the data to separate pipes used by the child processes as necessary.

2. Producer Consumer problem. Producer will produce a set of items into a FIFO, here text streams. Producer will keep on producing the text streams, while consumer will read from FIFO and display it on screen. Add a termination condition that when the producer wants to end this, it will write Bye and terminate which will be read by consumer which consequently will terminate as well.

3. Bi directional chat program. Use two FIFOs. If anyone types bye, their respective process terminates.