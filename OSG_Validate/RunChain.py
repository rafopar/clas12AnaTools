
import os
import sys
import glob


nArg = len(sys.argv);

if nArg != 7:
    print("\n");
    print("                  Number of Arguments is %d while it should be 7" %(nArg))
    print("Command should be: python3.9 RunChain.py Job_id1 \"Comment 1\"  Job_id2 \"Comment 2\"  Job_id3 \"Comment 3\"  ")
    print("An example command is python3.9 RunChain.py 101 \"Old\" 102 \"New New\" 103 \"New Old\" ")
    print("Exiting...")
    exit(1)
    
job1_id = int(sys.argv[1])
comment_1 = sys.argv[2]
job2_id = int(sys.argv[3])
comment_2 = sys.argv[4]
job3_id = int(sys.argv[5])
comment_3 = sys.argv[6]
    
print ("Job1 id is %d    Comment 1 is %s" %(job1_id, comment_1))
print ("Job2 id is %d    Comment 2 is %s" %(job2_id, comment_2))
print ("Job3 id is %d    Comment 3 is %s" %(job3_id, comment_3))

MergedFilesDir = "/volatile/clas12/rafopar/OSG_Validation/Merged_Outputs/"; # <--- Change to /volatile/clas12/ungaro/OSG_Validation/Merged_Outputs/
OSGBaseDir = "/volatile/clas12/osg2/rafopar" #   <--- change this to         OSGBaseDir = "/volatile/clas12/osg2/ungaro"

Job_IDs = [job1_id, job2_id, job3_id]
print (Job_IDs)

# ================= Looping over Job_IDs and merging files to a single file for the each job_id
for curJobID in Job_IDs:
        print("\n\n")
        MergedFileName = "%s/dst_%d.hipo" %(MergedFilesDir, curJobID )
        if not os.path.isfile(MergedFileName):
            print( "...........Merging Job %d hipo files............." %(curJobID) )
            os.system("hipo-utils -merge -o %s `find %s/job_%d/output/ -name dst.hipo` " %(MergedFileName, OSGBaseDir, curJobID))
        else:
            print( "The file %s already exists. So will not merge files from Job %d" %( MergedFileName, curJobID ))


        # ======= Running the analysis script on the merged file

        # Check however if the histograms root file already exists, if it exists, then will skip running the analysis script
        HistFileName = "Hists_OSGValidate_Job_%s.root" %(curJobID)
        if not os.path.isfile(HistFileName):
            print("Running AnaDDVCSOSG.exe for Job %d" %(curJobID))
            os.system("AnaDDVCSOSG.exe %d" %(curJobID) )
        else:
            print(" |")
            print("  =>The file %s already exist, so will skip running the AnaDDVCSOSG.exe on for Job %d " %(HistFileName, curJobID))


os.system(" root -l 'OSG_Validate.cc(%d, \"%s\", %d, \"%s\", %d, \"%s\")' " %(job1_id, comment_1, job2_id, comment_2, job3_id, comment_3 ) )
