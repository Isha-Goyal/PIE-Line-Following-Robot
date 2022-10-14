clear s

port = "COM4"; % adjust to your laptop's stuff

baudrate = 9600;
s = serialport(port, baudrate);

run = true;

data = [0, 0];

for i = 1:200
   if s.NumBytesAvailable > 0
       row = [readline(s), readline(s)];
       data(length(data)+1, :) = row;
   end

   pause(0.025);
end


plot(3:length(data), data(3:end,2), "*")