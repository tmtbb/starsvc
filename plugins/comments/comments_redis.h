//  Copyright (c) 2016-2017 The comments Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef COMMENTS_REDIS_H__
#define COMMENTS_REDIS_H__

#include "config/config.h"
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "comments/comments_proto_buf.h"

namespace comments_logic {

class CommentsRedis {
 public:
  CommentsRedis(config::FileConfig *config);
  virtual ~CommentsRedis();
 public: 
 bool WriteData(comments_logic::net_request::FansComments &fanscms);

 bool ReadCommentsData(const std::string& name, std::list<star_logic::Comments>& comments_list);
 bool ReadData( const std::string& name, std::list<std::string>& lst); 
 private:
  base_logic::DataEngine* redis_engine_;
};
}
#endif
