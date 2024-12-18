
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
using namespace std;

const int MAX_CHAR_NUM =
    100; 
struct User {
  unsigned int uid;   
  char name[MAX_CHAR_NUM];
  User *next; 
};

struct Like_Node{
    User *User_ptr;
    Like_Node *next;
};


struct Post {
  unsigned int pid;          
  char content[MAX_CHAR_NUM];        
  int num_of_likes;
  Like_Node* likes;            
};

User *create_user(const unsigned int uid, const char name[MAX_CHAR_NUM]) {
  User *new_user = new User;
  new_user->uid = uid;
  strcpy(new_user->name, name);
  new_user->next = nullptr;
  return new_user;
}

Post *create_post(const unsigned int pid,
                      const char content[MAX_CHAR_NUM]) {
  Post *new_post = new Post;
  new_post->pid = pid;
  strcpy(new_post->content, content);
  new_post->num_of_likes = 0;
  new_post->likes = nullptr;
  return new_post;
}


Post **dynamic_init_posts_array(const unsigned int posts_array_size) {
  Post **ret = nullptr;
  ret = new Post *[posts_array_size];
  for (int i = 0; i < posts_array_size; i++)
    ret[i] = nullptr;
  return ret;
}

bool search_user(User *head, const unsigned int uid, User *&prev,
                    User *&current) {
  prev = nullptr;
  for (current = head; current != nullptr; current = current->next) {
    if (current->uid == uid) {
    
      return true;
    } else if (current->uid > uid) {
      
      return false;
    }
    prev = current;
  }
  return false;
}

bool search_like(Like_Node *head, const unsigned int uid, Like_Node *&prev,
                      Like_Node *&current) {
  prev = nullptr;
  for (current = head; current != nullptr; current = current->next) {
    if (current->User_ptr->uid == uid) {
  
      return true;
    }
    prev = current;
  }
  return false;
}


bool search_post(Post **&posts_array, const unsigned int pid,
                   const unsigned int posts_array_size, int &i) {
  Post *post;
  if (posts_array != nullptr) {
    for (i = 0; i < posts_array_size; i++) {
      post = posts_array[i];
      if (post == nullptr) {
        break;
      }

      if (post->pid == pid) {
        
        return true;
      }
    }
  }
  return false;
}


void print_user(const User *user){
  cout<< "User Id: " << user->uid << ", name: " << user->name << "\n";
}



void print_post(const Post *post){
  cout<< "Post Id: " << post->pid << ", " << post->num_of_likes << " likes, content: " << post->content << "\n";
}


                    
bool add_user(User *&user_head, const unsigned int uid, const char name[MAX_CHAR_NUM]) {
    User *existing_user = user_head;
    User *last_user = nullptr;

    while (existing_user != nullptr) {
        if (existing_user->uid > uid) {
            break; 
        } else if (existing_user->uid == uid) {
            return false; 
        }
        last_user = existing_user;
        existing_user = existing_user->next;
    }

   
    User *added_user = create_user(uid, name);

   
    if (last_user != nullptr) {
        
        last_user->next = added_user;
        added_user->next = existing_user; 
    } else {
        
        added_user->next = user_head; 
        user_head = added_user; 
    }

    return true; 
}
                      
void display_users(User *user_head) {
  
  if (user_head == nullptr) {
    cout<< "No users in the system yet"<<"\n";
  }
  else {
    cout << "=== Users List ([uid, name]) ===" << endl;
    

    for (User *existing_user = user_head; existing_user != nullptr; existing_user = existing_user -> next) {
        print_user(existing_user);
    }
  }
}

bool add_post(Post **&posts_array, const unsigned int pid,
                const char content[MAX_CHAR_NUM], unsigned int &posts_array_size) {
  
    int i;
  for (int j = 0; j < posts_array_size; j++) {
    if (search_post(posts_array, pid, posts_array_size, i)) {
        return false;
    }
  }

  for (int j = 0; j < posts_array_size; j++) {
    if (posts_array != nullptr) {
        continue;
    }
    posts_array[j] = create_post(pid, content);
    return true;
  }
 
 int expanded_size = posts_array_size * 2;
 Post **expanded_array = new Post*[expanded_size];

 for (int j = 0; j < posts_array_size; j++) {
    expanded_array[j] = posts_array[j];
 }

for (int i = posts_array_size; i < expanded_size; i++) {
        expanded_array[i] = nullptr;
    }

    delete[] posts_array;
    posts_array = expanded_array; 
    posts_array_size = expanded_size; 

    
    for (int i = 0; i < posts_array_size; i++) {
        if (posts_array[i] == nullptr) {
            posts_array[i] = create_post(pid, content); 
            return true; 
        }
    }

    return false;

}



                   
void display_posts(Post **posts_array, const unsigned int posts_array_size) {
  
    for (int j = 0; j < posts_array_size; j++) {
        if (posts_array[j] != nullptr) {
            print_post(posts_array[j]);
        }
    }
}


bool add_like(User *&user_head, Post **&posts_array, unsigned int uid, unsigned int pid, const unsigned int posts_array_size) {
    
    User *user_prev = nullptr;
    User *user_current = nullptr;
    if (!search_user(user_head, uid, user_prev, user_current)) {
        return false; 
    }

    int post_index;
    if (!search_post(posts_array, pid, posts_array_size, post_index)) {
        return false; 
    }

    Post *post = posts_array[post_index];
    Like_Node *like_prev = nullptr;
    Like_Node *like_current = nullptr;
    
    if (search_like(post->likes, uid, like_prev, like_current)) {
        return false; 
    }

   
    Like_Node *new_like = new Like_Node;
    new_like->User_ptr = user_current; 
    new_like->next = post->likes;       
    post->likes = new_like;             


    post->num_of_likes++;

    return true; 
}


bool delete_like(User *user_head, Post **&posts_array, const unsigned int uid, const unsigned int pid, int posts_array_size) {
    User *existing_user = nullptr;
    User *previous_user = nullptr;
    Like_Node *last_like = nullptr;
    Like_Node *existing_like = nullptr;
    Post *existing_post = nullptr;

    if (search_user(user_head, uid, previous_user, existing_user)) {
        
        int post_index;
        if (search_post(posts_array, pid, posts_array_size, post_index)) {
            existing_post = posts_array[post_index];

    
            if (search_like(existing_post->likes, uid, last_like, existing_like)) {
                
                if (last_like == nullptr) {
                    existing_post->likes = existing_like->next; 
                } else {
                    last_like->next = existing_like->next; 
                }
                existing_post->num_of_likes--;
                delete existing_like;

                return true; 
            }
        } else {
            cout << "Failed to find the post" << endl;
        }
    } else {
        cout << "Failed to find the user" << endl;
    }

    return false; 
}

bool delete_user(User *&user_head, Post **&posts_array, const unsigned int uid, const unsigned int posts_array_size) {
    User *existing_user = user_head;
    User *last_user = nullptr;

    while (existing_user != nullptr && existing_user->uid != uid) {
        last_user = existing_user;
        existing_user = existing_user->next;
    }

    
    if (existing_user == nullptr) {
        cout << "Failed to find the user" << endl;
        return false;
    }
    
    for (int j = 0; j < posts_array_size; j++) {
        if (posts_array[j]) { 
            Like_Node *existing_user = posts_array[j]->likes;
            Like_Node *last_user = nullptr;

            while (existing_user) {
   
                if (existing_user->User_ptr->uid == uid) {
                    
                    if (last_user) {
                        last_user->next = existing_user->next; 
                    } else {
                        posts_array[j]->likes = existing_user->next; 
                    }
                    posts_array[j]->num_of_likes--; 
                    delete existing_user; 
                    existing_user = (last_user) ? last_user->next : posts_array[j]->likes; 
                } else {
                    last_user = existing_user; 
                    existing_user = existing_user->next;
                }
            }
        }
    }

    if (last_user) {
        last_user->next = existing_user->next;
    } else {
        user_head = existing_user->next; 
    }

    delete existing_user; 
    return true; 
}
                  
bool find_popular_post(Post **&posts_array, const int posts_array_size) {
    if (posts_array == nullptr || posts_array_size <= 0) {
        return false; 
    }

  
    Post *popular_post = nullptr;
    int max_likes = 0;

    for (int j = 0; j < posts_array_size; j++) {
        Post *existing_post = posts_array[j];
        if (existing_post != nullptr) {
            if (existing_post->num_of_likes > max_likes) {
                max_likes = existing_post->num_of_likes;
                popular_post = existing_post; 
            }
        }
    }

    
    if (popular_post) {
        print_post(popular_post);
        return true; 
    }

    return false;
}

void clean_up(User *&user_head, Post **&posts_array, unsigned int &posts_array_size) {
    if (posts_array != nullptr) {
        for (unsigned int j = 0; j < posts_array_size; j++) {
            if (posts_array[j] != nullptr) {
                Like_Node *current_like = posts_array[j]->likes;
                while (current_like != nullptr) {
                    Like_Node *temp_like = current_like;
                    current_like = current_like->next;
                    delete temp_like; 
                }
                delete posts_array[j]; 
                posts_array[j] = nullptr; 
            }
        }
        delete[] posts_array; 
        posts_array = nullptr; 
        posts_array_size = 0; 
    }
    while (user_head != nullptr) {
        User *temp_user = user_head;
        user_head = user_head->next; 
        delete temp_user; 
    }
    user_head = nullptr; 
}

int main() {
  enum MeunOption {
    OPTION_INSERT_USER = 0,
    OPTION_DISPLAY_USER_LIST,
    OPTION_INSERT_POST,
    OPTION_DISPLAY_POSTS,
    OPTION_INSERT_LIKE,
    OPTION_DELETE_LIKE,
    OPTION_DELETE_USER,
    OPTION_DISPLAY_POPULAR_POST,
    OPTION_EXIT_SYSTEM,
    MAX_MENU_OPTIONS
  };
  const int MAX_MENU_OPTIONS_LENGTH = 80;
  char menu_options[MAX_MENU_OPTIONS][MAX_MENU_OPTIONS_LENGTH] = {
      "Insert a new user",
      "Display Users",
      "Insert a new post",
      "Display Posts",      
      "Like a post",
      "Unlike a post",
      "Delete a user",
      "Display most popular post ",
      "Exit the system"};

  User *user_head = nullptr;
  Post **posts_array = nullptr;
  unsigned int posts_array_size = 0;
  int i, option;
  int uid, pid;
  char name[MAX_CHAR_NUM] = "";
  char content[MAX_CHAR_NUM] = "";
  bool ret = false;

  posts_array_size = 2;
  posts_array = dynamic_init_posts_array(posts_array_size);
  
  cout << "=== Simplified social network ===" << endl;
  while (true) {
    cout << "=== Menu ===" << endl;
    for (i = 0; i < MAX_MENU_OPTIONS; i++)
      cout << i + 1 << ": " << menu_options[i]
           << endl; 

    cout << "Enter your option: " << endl;
    cin >> option;
    option = option - 1; 

  
    if (option < 0 || option >= MAX_MENU_OPTIONS) {
      cout << "Invalid option" << endl;
      continue;
    }

    
    if (option == OPTION_EXIT_SYSTEM) {
      cout<< "Cleaning memory" << endl;
      clean_up(user_head, posts_array, posts_array_size);
      break; 
    }
    switch (option) {
    case OPTION_DISPLAY_USER_LIST:
      display_users(user_head);
      break;
    case OPTION_DISPLAY_POSTS:
      display_posts(posts_array, posts_array_size);
      break;
    case OPTION_INSERT_USER:
      uid = 0;
      cout << "Enter the user id: " << endl;
      cin >> uid;
      if (uid <= 0) {
        cout << "Enter a valid user id > 0" << endl;
        break;
      }
      cout << "Enter a name: " << endl;
      cin >> name;

      ret = add_user(user_head, uid, name);
      if (ret == false) {
        cout << "Failed to insert user " << uid << endl;
      } else {
        cout << uid << " inserted to the network" << endl;
      }
      break;
    case OPTION_INSERT_POST:
      pid = 0;
      cout << "Enter the post id: " << endl;
      cin >> pid;
      if (pid <= 0) {
        cout << "Enter a valid post id > 0" << endl;
        break;
      }
      cout << "Add content: " << endl;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cin.getline(content, MAX_CHAR_NUM);

      ret = add_post(posts_array, pid, content, posts_array_size);
      if (ret == false) {
        cout << "Failed to insert post " << pid << endl;
      } else {
        cout << pid << " successfully posted" << endl;
      }
      break;
    case OPTION_INSERT_LIKE:
      uid = 0;
      cout << "Enter the user id: " << endl;
      cin >> uid;
      if (uid <= 0) {
        cout << "Enter a valid user id > 0" << endl;
        break;
      }
      pid = 0;
      cout << "Enter the post id: " << endl;
      cin >> pid;
      if (pid <= 0) {
        cout << "Enter a valid post id > 0" << endl;
        break;
      }
      
      ret = add_like(user_head,posts_array, uid, pid, posts_array_size);
      if (ret == false) {
        cout << "Failed to like, uid: " << uid
             << ", pid: " << pid << endl;
      } else {
        cout << "A like is successfully inserted" << endl;
      }
      break;
    case OPTION_DELETE_LIKE:
      uid = 0;
      cout << "Enter the user id: " << endl;
      cin >> uid;
      if (uid <= 0) {
        cout << "Enter a valid user id > 0" << endl;
        break;
      }
      pid = 0;
      cout << "Enter the post id: " << endl;
      cin >> pid;
      if (pid <= 0) {
        cout << "Enter a valid post id > 0" << endl;
        break;
      }

      ret = delete_like(user_head, posts_array, uid, pid,
                             posts_array_size);
      if (ret == false) {
        cout << "Failed to delete like, uid: " << uid
             << ", pid: " << pid << endl;
      } else {
        cout << "The like is successfully deleted" << endl;
      }
      break;
    case OPTION_DELETE_USER:
      uid = 0;
      cout << "Enter the user id: " << endl;
      cin >> uid;
      if (uid <= 0) {
        cout << "Enter a valid post id > 0" << endl;
        break;
      }

      ret = delete_user(user_head, posts_array, uid, posts_array_size);
      if (ret == false) {
        cout << "Failed to delete user, user id: " << uid << endl;
      } else {
        cout << "user successfully deleted" << endl;
      }
      break;
    case OPTION_DISPLAY_POPULAR_POST:
      
      ret = find_popular_post(posts_array, posts_array_size);
      if(ret == false){
        cout << "No posts in the network yet" << endl;
      }
      break;
    default:
      break;
    } 
  }

  return 0;
}