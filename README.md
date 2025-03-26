# Terminal-Chat
a C++ app for Linux lets you text over a local network. Double-click to launch, create a profile, and choose server or client mode. Server sends first; client views. After connecting, both can message freely. Try it for details!

# Terminal Chat - User Manual  

## Installation Process  

1. *Download the Files*  
   - Download the ZIP file from the GitHub repository or clone the repository using the terminal:  
     bash
     git clone <repository_link>
       
   - If you downloaded the ZIP file, extract it.  
   - If you cloned the repository, navigate to the cloned folder:  
     bash
     cd <cloned_folder>
       

2. *Setting Up the Desktop Entry*  
   - Copy the Terminal Chat.desktop file and paste it on your desktop.  
   - Open the Terminal Chat.desktop file using Notepad or a text editor.  
   - Modify the following paths inside the file:  
     ini
     Exec=gnome-terminal -- <ChatUpdate file path>
     Icon=<Icon file path>
     Path=<Folder path where ChatUpdate.cpp is stored>
       
   - Save the file after making the necessary changes.  

3. *Granting Execution Permission (If Needed)*  
   - If the program does not run on the first attempt, grant execution permission to the ChatUpdate file by opening a terminal in the folder where it is located and running:  
     bash
     chmod +x ChatUpdate
       
   - Now, you should be able to use it without any issues.  

4. *Final Steps*  
   - Keep the Terminal Chat.desktop file on your desktop.  
   - The rest of the files can be stored in any directory.  
   - When you open the desktop file, it will load the main files from the specified directory.  

---

# টার্মিনাল চ্যাট - ব্যবহার নির্দেশিকা  

## ইনস্টলেশন প্রক্রিয়া  

1. *ফাইল ডাউনলোড করুন*  
   - GitHub রিপোজিটরি থেকে ZIP ফাইল ডাউনলোড করুন বা টার্মিনালে নিচের কমান্ড চালিয়ে ক্লোন করুন:  
     bash
     git clone <repository_link>
       
   - যদি ZIP ফাইল ডাউনলোড করে থাকেন, তাহলে তা আনজিপ করুন।  
   - যদি রিপোজিটরি ক্লোন করে থাকেন, তাহলে নিচের কমান্ড দিয়ে ফোল্ডারে প্রবেশ করুন:  
     bash
     cd <cloned_folder>
       

2. *ডেস্কটপ এন্ট্রি সেটআপ করুন*  
   - Terminal Chat.desktop ফাইলটি কপি করে ডেস্কটপে পেস্ট করুন।  
   - Notepad বা অন্য কোনো টেক্সট এডিটরের মাধ্যমে Terminal Chat.desktop ফাইলটি খুলুন।  
   - নিচের *তিনটি ফাইল পাথ* সংশোধন করুন:  
     ini
     Exec=gnome-terminal -- <ChatUpdate ফাইলের পাথ>
     Icon=<আইকন ফাইলের পাথ>
     Path=<ChatUpdate.cpp যেখানে রাখা আছে>
       
   - পরিবর্তন করার পর ফাইলটি সংরক্ষণ করুন।  

3. *এক্সিকিউশন পারমিশন দিন (যদি প্রয়োজন হয়)*  
   - যদি প্রথমবার চালাতে সমস্যা হয়, তাহলে ChatUpdate ফাইলটিকে এক্সিকিউট পারমিশন দিতে হবে।  
   - যেখানে ফাইলটি রয়েছে, সেখানে টার্মিনাল খুলুন এবং নিচের কমান্ড চালান:  
     bash
     chmod +x ChatUpdate
       
   - এরপর এটি সহজেই চালানো যাবে।  

4. *চূড়ান্ত ধাপ*  
   - Terminal Chat.desktop ফাইলটি ডেস্কটপে রাখুন।  
   - বাকি ফাইলগুলো যেকোনো ফোল্ডারে রাখতে পারেন।  
   - ডেস্কটপ ফাইলটি চালালে, এটি নির্দিষ্ট ডিরেক্টরি থেকে মেইন ফাইলগুলো লোড করবে।
