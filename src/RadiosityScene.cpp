

//kdBox *createMergedBoundingBox(std::vector<Triangle> *sortedObjects, int start, int end) {
//    kdBox *box = new kdBox();
//    for (int i = start; i < end; i++) {
//        box->merge((*sortedObjects)[i].getBoundingBox());
//    }
//    return box;
//}

//std::vector<Triangle> *copyVector(vector<Triangle> *v) {
//    std::vector<Triangle> *newV = new std::vector<Triangle>;
//    for (int i = 0; i < v->size(); i++) {
//        newV->push_back((*v)[i]);
//    }
//    return newV;
//}
//
//void kdTree::intersectedObjects(glm::vec3 position, glm::vec3 direction, kdTree::Node *curNode, std::vector<Triangle> &v) {
//    const double z = 0.0;
//    if (curNode->box->intersect(position, direction, const_cast<double &>(z), const_cast<double &>(z))) {
//        if (curNode->isLeaf()) {
//            v.push_back((*(curNode->sceneObject))[0]);
//        }
//        else {
//            intersectedObjects(position, direction, curNode->left, v);
//            intersectedObjects(position, direction, curNode->right, v);
//        }
//    }
//}
//
//void kdTree::buildTree(kdBox box) {
//    root->box = new kdBox(box.getMin(), box.getMax());
//    subBuildTree(root, 0);
//}
//
//kdTree kdTree::subBuildTree(kdTree::Node *parent, int depth) {
//
//    std::vector<Triangle> *objects = parent->sceneObject;
//
//    if (parent->sceneObject->size() != 1) { //&& depth == 10
//        // X DIMENSION
//
//        int size = objects->size();
//
//        std::vector<Triangle> *sortedXObjects = copyVector(objects);
//        std::sort(sortedXObjects->begin(),
//                  sortedXObjects->end(),
//                  [](Triangle lhs, Triangle rhs) {
//                      return lhs.getBoundingBox().getMin()[0] < rhs.getBoundingBox().getMin()[0];
//                  });
//        kdBox *totalXBox = createMergedBoundingBox(sortedXObjects, 0, sortedXObjects->size());
//        kdBox *leftXHalf = createMergedBoundingBox(sortedXObjects, 0, sortedXObjects->size() / 2);
//        kdBox *rightXHalf = createMergedBoundingBox(sortedXObjects, sortedXObjects->size() / 2,
//                                                    sortedXObjects->size());
//        double sumOfX = leftXHalf->area() + rightXHalf->area();
//        //double sumOfX = *sortedXObjects[size - 1]->getBoundingBox().getMax[0] - *sortedXObjects[0]->getBoundingBox().getMin[0];
//
//        // Y DIMENSION
//        std::vector<Triangle> *sortedYObjects = copyVector(objects);
//        std::sort(sortedYObjects->begin(),
//                  sortedYObjects->end(),
//                  [](Triangle lhs, Triangle rhs) {
//                      return lhs.getBoundingBox().getMin()[1] < rhs.getBoundingBox().getMin()[1];
//                  });
//        kdBox *totalYBox = createMergedBoundingBox(sortedYObjects, 0, sortedYObjects->size());
//        kdBox *leftYHalf = createMergedBoundingBox(sortedYObjects, 0, sortedYObjects->size() / 2);
//        kdBox *rightYHalf = createMergedBoundingBox(sortedYObjects, sortedYObjects->size() / 2,
//                                                    sortedYObjects->size());
//        double sumOfY = leftYHalf->area() + rightYHalf->area();
//
//        // Z DIMENSION
//        std::vector<Triangle> *sortedZObjects = copyVector(objects);
//        std::sort(sortedZObjects->begin(),
//                  sortedZObjects->end(),
//                  [](Triangle lhs, Triangle rhs) {
//                      return lhs.getBoundingBox().getMin()[2] < rhs.getBoundingBox().getMin()[2];
//                  });
//        kdBox *totalZBox = createMergedBoundingBox(sortedZObjects, 0, sortedZObjects->size());
//        kdBox *leftZHalf = createMergedBoundingBox(sortedZObjects, 0, sortedZObjects->size() / 2);
//        kdBox *rightZHalf = createMergedBoundingBox(sortedZObjects, sortedZObjects->size() / 2,
//                                                    sortedZObjects->size());
//        double sumOfZ = leftZHalf->area() + rightZHalf->area();
//
//        // Pick best dimension
//        kdBox *bestLeft;
//        kdBox *bestRight;
//        std::vector<Triangle> *bestList;
//
//        if (sumOfX < sumOfY && sumOfX < sumOfZ) {
//            bestLeft = leftXHalf;
//            bestRight = rightXHalf;
//            bestList = sortedXObjects;
//        } else if (sumOfY < sumOfX && sumOfY < sumOfZ) {
//            bestLeft = leftYHalf;
//            bestRight = rightYHalf;
//            bestList = sortedYObjects;
//        } else {
//            bestLeft = leftZHalf;
//            bestRight = rightZHalf;
//            bestList = sortedZObjects;
//        }
//
//        //for (int i = 0; i < bestLeft.size() / 2)
//        std::vector<Triangle> *vLeft = new std::vector<Triangle>;
//        for (int i = 0; i < bestList->size() / 2; i++) {
//            vLeft->push_back((*bestList)[i]);
//        }
//        parent->left = new kdTree::Node(NULL, NULL, bestLeft, vLeft);
//        subBuildTree(parent->left, depth + 1);
//
//        std::vector<Triangle> *vRight = new std::vector<Triangle>;
//        for (int i = bestList->size() / 2; i < bestList->size(); i++) {
//            vRight->push_back((*bestList)[i]);
//        }
//        parent->right = new kdTree::Node(NULL, NULL, bestRight, vRight);
//        subBuildTree(parent->right, depth + 1);
//    }
//}
//
