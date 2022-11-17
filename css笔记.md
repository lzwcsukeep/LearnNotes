css笔记

简要记录

1. element : 元素。构成html 页面的

2. properties ： 元素的特性，如color .

元素的box 模型：

每一个元素都可以看成一个方块。从内到外有content,padding,border,margin.

元素的width,height 属性定义的都是content的宽高。一个元素所占区域的真正大小是content+padding+border+margin.

![](E:\Files\LearnNotes\imgs\css_box_model.png)

Explanation of the different parts:

- **Content** - The content of the box, where text and images appear
- **Padding** - Clears an area around the content. The padding is transparent
- **Border** - A border that goes around the padding and content
- **Margin** - Clears an area outside the border. The margin is transparent

The box model allows us to add a border around elements, and to define space between elements.

**Important:** When you set the width and height properties of an element with CSS, you just set the width and height of the **content area**. To calculate the full size of an element, you must also add padding, borders and margins.

```css
div {
  width: 320px;
  padding: 10px;
  border: 5px solid gray;
  margin: 0;
}
```

div 的真正大小是350px = 320px (width) + 2\*10px(padding) + 2\*5px(border) .

border ,margin 可以还可细分指定上下左右。

更多关于box model 看下面链接：

https://www.w3schools.com/css/css_boxmodel.asphttps://www.w3schools.com/css/css_boxmodel.asp
