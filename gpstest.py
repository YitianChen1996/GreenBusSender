import gps

# Listen on port 2947 (gpsd) of localhost
session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)
i = 0

while True:
    try:
        
        i = i + 1
        if i > 100:
            i = 0
    	report = session.next()
		# Wait for a 'TPV' report and display the current time
		# To see all report data, uncomment the line below
	output_file = open("AdafruitGPSdata.txt", "w");
        if (report['class'] == 'TPV' and hasattr(report, 'lat') and hasattr(report, 'lon') and hasattr(report, 'speed') and hasattr(report, 'track')):
            print 'AdafruitGPS: got valid data'
            output_file.write(str(i)+' '+str(report.lat)+' '+ str(report.lon)+' '+ str(report.speed)+' '+ str(report.track))
        else:
            print 'AdafruitGPS: no valid data yet'
            output_file.write(str(i)+' 0 0 0 0');
        output_file.close();
    except KeyError:
		pass
    except KeyboardInterrupt:
		quit()
    except StopIteration:
		session = None
		print "GPSD has terminated"
