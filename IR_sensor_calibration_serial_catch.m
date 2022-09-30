clear s
port = "COM4"; % adjust to your laptop's stuff

audrate = 9600;
s = serialport(port, buadrate);

run = true;

data = [0, 0];

while (run)
   if s.NumBytesAvailable > 0
       row = [readline(s), readline(s)];
       data(length(data)+1, :) = row;
   end
   
   pause(0.25);
end