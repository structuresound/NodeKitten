//
//  SceneController.h
//  Pods
//
//  Created by Leif Shackelford on 5/31/15.
//
//

#ifndef __CollectionView__
#define __CollectionView__

#include "ScrollView.h"
#include "DynamicDataSource.h"

class CollectionView;

class CollectionViewDelegate {
public:
    void didSelectItemAtIndex(IndexPath index, CollectionView *view);
    void didDeSelectItemAtIndex(IndexPath index, CollectionView *view);
    
    void didScrollToPosition(V2 position, CollectionView *view);
    void didFinishScrollingToPosition(V2 position, CollectionView *view);
};

class CollectionView : public ScrollView, public DynamicDataSource, public CollectionViewDelegate {
    
    DynamicDataSource * _dataSource = this;
    CollectionViewDelegate * _collectionViewDelegate = this;
    
public:
    // Convenience Constructors
    static shared_ptr<CollectionView> viewWithSize(V2 size);
    
    CollectionView(V2 size) : ScrollView(size) {}
    ~CollectionView(){};
    
    function<bool(Json::Value a, Json::Value b)> sortBlock;
    
    // DELEGATE
    CollectionViewDelegate *delegate = this;

    vector<U1t> selectedIndexes;

    View* viewAtIndex(IndexPath index);
    
    bool verticalScrollEnabled {true};
    bool horizontalScrollEnabled {false};
    
    // ANIMATIONS
    
    virtual void updateWithTimeSinceLast(F1t dt) override;
    virtual void didScroll(U1 index) override;
    virtual void didFinishScrolling() override;
    
    virtual void scrollToItemAtIndex(U1 index);
    virtual void scrollToView(View *view);
    
    // GENERATION
    
    virtual View* createViewForItemAtIndex(U1 index);
    View* viewForItemAtIndex(U1 index);

    void animateChanges();
    
};

#endif
